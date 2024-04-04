#ifndef _IMU_H_
#define _IMU_H_

#include "wit_c_sdk.h"
#include "REG.h"

#include <thread>
#include <mutex>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include "defines.hpp"
#include <vector>
#include <Fusion.h>

using namespace std;

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80

#define BIAS_UPDATE 0x10
#define SETTTINGS_UPDATE 0x20

// Static variables needed by Wit API
extern char s_cDataUpdate;
extern int fd;    
extern SerialWrite p_WitSerialWriteFunc;
extern DelaymsCb p_WitDelaymsFunc;

class Imu{
    public:
        Imu() : m_fKeepRunning(true), m_mutex(), m_biasTable(9,0), m_settings(5,0),
        m_accel{0,0,0}, m_gyro{0,0,0}, m_mag{0,0,0},
        gyroscopeMisalignment{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        gyroscopeSensitivity{1.0f, 1.0f, 1.0f},
        gyroscopeOffset{0.0f, 0.0f, 0.0f},
        accelerometerMisalignment{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        accelerometerSensitivity{1.0f, 1.0f, 1.0f},
        accelerometerOffset{0.0f, 0.0f, 0.0f},
        softIronMatrix{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        hardIronOffset{0.0f, 0.0f, 0.0f}, 
        offset()
        {
            WitInit(WIT_PROTOCOL_NORMAL, 0x50);

            WitSerialWriteRegister([](uint8_t *data, uint32_t len){
	            int nWritten = write(fd, data, len*sizeof(unsigned char));
                // len = nWritten;
            });

            WitDelayMsRegister([](uint16_t ms){
	            usleep(ms * 1000);
            });
   
            WitRegisterCallBack([](uint32_t uiReg, uint32_t uiRegNum){
                for(int i = 0; i < uiRegNum; i++){
                    switch(uiReg)
                    {
                        case AZ: 
                            s_cDataUpdate |= ACC_UPDATE; break;
                        case GZ: 
                            s_cDataUpdate |= GYRO_UPDATE; break;
                        case HZ: 
                            s_cDataUpdate |= MAG_UPDATE; break;
                        case Yaw: 
                            s_cDataUpdate |= ANGLE_UPDATE; break;
                        case AXOFFSET: case AYOFFSET: case AZOFFSET: case GXOFFSET: case GYOFFSET: case GZOFFSET: case HXOFFSET: case HYOFFSET: case HZOFFSET: 
                            s_cDataUpdate |= BIAS_UPDATE; break;
                        case RSW: case RRATE: case BAUD: case AXIS6: case BANDWIDTH:
                            s_cDataUpdate |= SETTTINGS_UPDATE; break;
                        
                        default: 
                            s_cDataUpdate |= READ_UPDATE; break;
                    }
                    uiReg++;
                }
                
            });
            FusionAhrsInitialise(&m_fusion);
                // Set AHRS algorithm settings
            const FusionAhrsSettings settings = {
                .convention = FusionConventionNed,
                .gain = 0.5f,
                .gyroscopeRange = 2000.0f, 
                .accelerationRejection = 10.0f,
                .magneticRejection = 10.0f,
                .recoveryTriggerPeriod = 5 * (IMU_SAMPLE_INTERVAL_US /1000000), /* 5 seconds */
            };
            FusionAhrsSetSettings(&m_fusion, &settings);
            m_lastImuUpdate =  chrono::high_resolution_clock::now();
            serial_open(IMU_SERIAL_PORT, 9600);
        }
        
        void start();
        void stop();

        inline bool isKeepRunning(){return m_fKeepRunning;}

        // Fusion APIs
        inline FusionVector getLinearAcceleration(){return FusionAhrsGetLinearAcceleration(&m_fusion);}
        inline FusionQuaternion getQuaternion (){return FusionAhrsGetQuaternion(&m_fusion);}
        inline  FusionEuler quaternionToEuler(const FusionQuaternion quaternion){return FusionQuaternionToEuler(quaternion);}
        // IMU APIs
        inline int witStartAccCali(){return WitStartAccCali();}
        inline int witStopAccCali(){return WitStopAccCali();}
        inline int witStartMagCali(){return WitStartMagCali();}
        inline int witStopMagCali(){return WitStopMagCali();}
        inline int witSetUartBaud(int uiBaudIndex){return WitSetUartBaud(uiBaudIndex);}
        inline int witSetBandwidth(int uiBaudWidth){return WitSetBandwidth(uiBaudWidth);}
        inline int witSetOutputRate(int uiRate){return WitSetOutputRate(uiRate);}
        inline int witSetContent(int uiRsw){return WitSetContent(uiRsw);}
        inline int witSetCanBaud(int uiBaudIndex){return WitSetCanBaud(uiBaudIndex);}
        inline int witSaveParameter(){return WitSaveParameter();}
        inline int witSetForReset(){return WitSetForReset();}
        inline int witCaliRefAngle(){return WitCaliRefAngle();}
        inline int witWriteReg(uint32_t reg, uint16_t value){return WitWriteReg(reg, value);}
        inline int witReadReg(uint32_t reg, uint32_t len){return WitReadReg(reg, len);}
        void readBiases();
        void getBiasTable(vector<uint16_t> &offsets);
        void readSettings();
        void getSettings(vector<uint16_t> &settings);
        void setAxis6(int on);
        void setBandwidth(int bw);

    private:
        void addMeasurements(uint flags);
        void AutoScanSensor(char* dev);
        static int serial_open(const char *dev, int baud);
        bool m_fKeepRunning;
        thread m_thread;

        vector<uint16_t> m_biasTable;
        vector<uint16_t> m_settings;
        timed_mutex m_mutex;
        
        FusionAhrs m_fusion;
        FusionVector m_accel;
        FusionVector m_gyro;
        FusionVector m_mag;

        FusionMatrix gyroscopeMisalignment;
        FusionVector gyroscopeSensitivity;
        FusionVector gyroscopeOffset;
        FusionMatrix accelerometerMisalignment;
        FusionVector accelerometerSensitivity;
        FusionVector accelerometerOffset;
        FusionMatrix softIronMatrix;
        FusionVector hardIronOffset;
        FusionOffset offset;

        chrono::_V2::system_clock::time_point m_lastImuUpdate;
};



#endif