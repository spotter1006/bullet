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

        inline bool isKeepSweeping(){return m_fKeepSweeping;}
        inline void setKeepSweeping(bool val){m_fKeepSweeping=val;}
        inline int getLeftSweepLimit(){return m_nLeftSweepLimit;}
        inline int getRightSweepLimit(){return m_nRightSweepLimit;}
        inline int step(int dir){return m_stepper.step(dir);}
        inline int getDirection(){return m_stepper.getDirection();}
        inline int getStep(){return m_stepper.getPosition();}
        inline int getRadius(){return m_nRadius;}
        inline void setChaserPattern(vector<ws2811_led_t> &pattern){m_chaser.setPattern(pattern);}
        inline void chaserRotate(int direction){m_chaser.rotate(direction);}

    private:
        Stepper m_stepper;
        Chaser m_chaser;
        bool m_fKeepSweeping;
        int m_nLeftSweepLimit;
        int m_nRightSweepLimit;
        int m_nRadius;

        vector<thread> m_threads;

};
#endif

