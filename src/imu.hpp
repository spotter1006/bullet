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

using namespace std;

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80

// Static variables needed by Wit API
extern char s_cDataUpdate;
extern int fd;    
extern SerialWrite p_WitSerialWriteFunc;
extern DelaymsCb p_WitDelaymsFunc;


class Imu{
    public:
        Imu() : m_fKeepRunning(true), m_headingHistogram(HEADING_BUCKETS,0), m_nHeading(0), m_mutex()        
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
                for(int i = 0; i < uiRegNum; i++)
                {
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
                        case TEMP:
                            break;
                        case AX: case AY: case GX: case GY: case HX: case HY: case Roll: case Pitch: case VERSION:
                            break;
                        default: 
                            s_cDataUpdate |= READ_UPDATE; break;
                    }
                    uiReg++;
                }
            });
            serial_open(IMU_SERIAL_PORT, 115200);
        }
        static int serial_open(const char *dev, int baud);
        void start();
        void stop();
        void addMeasurements(int flags);
        void decrementHistograms(int dec);
        inline int getHeading(){return m_nHeading;}
        int getHeadingChange(int window);
        inline bool isKeepRunning(){return m_fKeepRunning;}
        
        // Wrap all wit APIs
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
        inline int witWriteRegister(uint32_t reg, uint16_t value){return WitWriteReg(reg, value);}
    private:
        void AutoScanSensor(char* dev);
        bool m_fKeepRunning;
        thread m_thread;
        vector<int> m_headingHistogram;
        int m_nHeading;
        timed_mutex m_mutex;
};



#endif