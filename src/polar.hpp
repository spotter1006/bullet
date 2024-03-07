#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>
#include <vector>
#include <thread>
#include <signal.h>
#include <sys/time.h>
#include "chaser.hpp"
using namespace std;

using namespace std;

class Polar{
    public:
        Polar(int left, int right, int radius);

        void start();
        void stop();

        inline int getRadius(){return m_nRadius;}
        inline bool isKeepSweeping(){return m_fKeepSweeping;}
        inline void setKeepSweeping(bool val){m_fKeepSweeping=val;}
        inline int getLeftSweepLimit(){return m_nLeftSweepLimit;}
        inline int getRightSweepLimit(){return m_nRightSweepLimit;}
        inline int getAngle(){return m_nAngle;}
        inline void setAngle(int angle){m_nAngle = angle;}
        inline int getInterval(){return m_nInterval;}
        inline void setIternval(int interval){m_nInterval = interval;}

        // Stepper pass-throughs
        inline int stepMotor(int dir){return m_stepper.step(dir);}
        inline int getMotorDirection(){return m_stepper.getMotorDirection();}
        inline int getMotorPosition(){return m_stepper.getPosition();}
        
        // Chaser pass-throughs
        inline void setChaserPattern(vector<ws2811_led_t> &pattern){m_chaser.setPattern(pattern);}
        inline void chaserRotate(int direction){m_chaser.rotate(direction);}
  
    private:
        Stepper m_stepper;
        Chaser m_chaser;
        
        bool m_fKeepSweeping;
        int m_nLeftSweepLimit;
        int m_nRightSweepLimit;
        int m_nRadius;

        int m_nInterval;            // Interval between pattern shifts of animation in units of calls to chaser.rotate(). Bipolar, aslo tess rotate direction
        int m_nAngle;              // Angle of chaser bar in motor steps

        vector<thread> m_threads;

};
#endif

