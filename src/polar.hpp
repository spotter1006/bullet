#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>
#include <vector>
#include<list>
#include <map>
#include <thread>
#include <signal.h>
#include <sys/time.h>
#include "chaser.hpp"
#include "imu.hpp"
using namespace std;

using namespace std;


class Polar{
    public:

        Polar(int radius): m_chaser(), m_imu(), m_fkeepRunning(true){}

        void start();
        void stop();
        // void home();

        inline bool iskeepRunning(){return m_fkeepRunning;}
        inline void setkeepRunning(bool val){m_fkeepRunning=val;}

        // Chaser pass-throughs
        inline void setHue(int hue){m_chaser.setHue(hue);}
        inline int getChaserInterval(){return m_chaser.getInterval();}
        inline void setChaserInterval(int interval){m_chaser.setInterval(interval);}
 
        // Stepper pass-throughs
        inline int stepMotor(int dir){return m_stepper.step(dir);}
        inline int getMotorDirection(){return m_stepper.getMotorDirection();}
        inline int getMotorPosition(){return m_stepper.getPosition();}
        inline void zeroMotorAngle(){m_stepper.zeroPosition();}
        inline void enableMotor(bool enable){m_stepper.enable(enable);}
        inline void setMotorTargetPosition(int position){m_stepper.setTargetPosition(position);}
        

        // IMU pass-throughs
        inline int witStartAccCali(){return m_imu.witStartAccCali();}
        inline int witStopAccCali(){return m_imu.witStopAccCali();}
        inline int witStartMagCali(){return m_imu.witStartMagCali();}
        inline int witStopMagCali(){return m_imu.witStopMagCali();}
        inline int witSetUartBaud(int uiBaudIndex){return m_imu.witSetUartBaud(uiBaudIndex);}
        inline int witSetBandwidth(int uiBaudWidth){return m_imu.witSetBandwidth(uiBaudWidth);}
        inline int witSetOutputRate(int uiRate){return m_imu.witSetOutputRate(uiRate);}
        inline int witSetContent(int uiRsw){return m_imu.witSetContent(uiRsw);}
        inline int witSetCanBaud(int uiBaudIndex){return m_imu.witSetCanBaud(uiBaudIndex);}
        inline int witSaveParameter(){return m_imu.witSaveParameter();}
        inline int witSetForReset(){return m_imu.witSetForReset();}
        inline int witCaliRefAngle(){return m_imu.witCaliRefAngle();}

        //Fusion pass-throughs
        inline FusionVector getLinearAcceleration(){return m_imu.getLinearAcceleration();}
        inline FusionQuaternion getQuaternion (){return m_imu.getQuaternion();}
        inline FusionEuler quaternionToEuler(const FusionQuaternion quaternion){return m_imu.quaternionToEuler(quaternion);}
        
  
    private:
        Stepper m_stepper;
        Chaser m_chaser;
        Imu m_imu;
        
        bool m_fkeepRunning;

        vector<thread> m_threads;


};
#endif


