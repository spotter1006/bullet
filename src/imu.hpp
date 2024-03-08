#ifndef _IMU_H_
#define _IMU_H_

#include "serial.h"
#include "wit_c_sdk.h"
#include "REG.h"
#include <stdint.h>
#include <thread>

using namespace std;

#define ACC_UPDATE		0x01
#define GYRO_UPDATE		0x02
#define ANGLE_UPDATE	0x04
#define MAG_UPDATE		0x08
#define READ_UPDATE		0x80


static int fd, s_iCurBaud = 9600;
static volatile char s_cDataUpdate = 0;


const int c_uiBaud[] = {2400 , 4800 , 9600 , 19200 , 38400 , 57600 , 115200 , 230400 , 460800 , 921600};


static void AutoScanSensor(char* dev);
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
static void Delayms(uint16_t ucMs);

class Imu{
    public:
        Imu() : m_fKeepRunning(true){}
        void setup(int baudIndex, int updateRate);
        void startMagCalibration(){WitStartMagCali();}
        void stopMagCalibration(){WitStopMagCali();}
        void startAccCalibration(){WitStartAccCali();}
        void stopAccCalibration(){WitStopAccCali();}
        void start();
        void stop();
        inline bool isKeepRunning(){return m_fKeepRunning;}
    private:
        bool m_fKeepRunning;
        thread m_thread;
};



#endif