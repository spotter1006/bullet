#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>
#include <vector>
#include <thread>
#include <signal.h>
#include <sys/time.h>
#include "chaser.hpp"
#include "imu.hpp"
using namespace std;

using namespace std;

const ws2811_led_t colorGradient[] = {                       // https://colordesigner.io/gradient-generator
    0x00fe0101, 0x00fe2302, 0x00fe4503, 0x00fe6704,     // green to red
    0x00fe8805, 0x00feaa06, 0x00fecb07, 0x00feec08,
    0x00f1ff08, 0x00d1ff09, 0x00b1ff0a, 0x0091ff0b,
    0x0071ff0c, 0x0052ff0d, 0x0033ff0e, 0x0014ff0f
};

class Polar{
    public:
        Polar(int left, int right, int radius);
        void setIntensities(vector<ws2811_led_t> intensities){m_intensities = intensities;}
        void setHue(int hue);
        void start();
        void stop();
        void home();
        void decrementHistogram();
        int addHeading(float heading);
        int getHeadingVariance(int width);
        void clearHistory();

        inline int getRadius(){return m_nRadius;}
        inline bool isKeepSweeping(){return m_fKeepSweeping;}
        inline void setKeepSweeping(bool val){m_fKeepSweeping=val;}
        inline int getLeftSweepLimit(){return m_nLeftSweepLimit;}
        inline int getRightSweepLimit(){return m_nRightSweepLimit;}
        inline int getAngle(){return m_nAngle;}
        inline void setAngle(int angle){m_nAngle = angle;}
        inline int getInterval(){return m_nInterval;}
        inline void setIterval(int interval){m_nInterval = interval;}

        // Stepper pass-throughs
        inline int stepMotor(int dir){return m_stepper.step(dir);}
        inline int getMotorDirection(){return m_stepper.getMotorDirection();}
        inline int getMotorPosition(){return m_stepper.getPosition();}
        inline void zeroMotorAngle(){m_stepper.zeroPosition();}
        inline void enableMotor(bool enable){m_stepper.enable(enable);}
        
        // Chaser pass-throughs
        inline void chaserRotate(int direction){m_chaser.rotate(direction);}

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
        inline FusionVector getLinearAcceleration(){return m_imu.getLinearAcceleration();}
        inline FusionQuaternion getQuaternion (){return m_imu.getQuaternion();}
        inline FusionEuler quaternionToEuler(const FusionQuaternion quaternion){return m_imu.quaternionToEuler(quaternion);}
        
  
    private:
        Stepper m_stepper;
        Chaser m_chaser;
        Imu m_imu;
        
        bool m_fKeepSweeping;
        int m_nLeftSweepLimit;
        int m_nRightSweepLimit;
        int m_nRadius;

        int m_nInterval;            // Interval between pattern shifts of animation in units of calls to chaser.rotate(). Bipolar, aslo tess rotate direction
        int m_nAngle;              // Angle of chaser bar in motor steps

        vector<thread> m_threads;
        inline ws2811_led_t redToGreen(int val){return colorGradient[(val + 128) / 16];}

        vector<ws2811_led_t> m_intensities;
        vector<ws2811_led_t> m_colors;
        vector<int> m_headings;
        int m_nCurrentHeading;

};
#endif

