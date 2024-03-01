#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>
#include "frame.hpp"
#include <vector>
#include <thread>

class Polar{
    public:
        Polar();
        ~Polar();

        void start(int left, int right, int stepIntervalUs);
        void stop();

        inline ws2811_t* getLedString(){return &m_ledstring;}
        inline bool isKeepSweeping(){return m_fKeepSweeping;}
        inline void setKeepSweeping(bool val){m_fKeepSweeping=val;}
        inline int getLeftSweepLimit(){return m_nLeftSweepLimit;}
        inline int getRightSweepLimit(){return m_nRightSweepLimit;}
        inline int step(int dir){return m_stepper.step(dir);}
        inline int getStep(){return m_stepper.getPosition();}
        inline void getBar(int step, Bar& bar){return m_frame.getBar(step, bar);}

    private:
        Stepper m_stepper;
        ws2811_t m_ledstring;
        Frame m_frame;
        bool m_fKeepSweeping;
        int m_nLeftSweepLimit;
        int m_nRightSweepLimit;

        vector<thread> m_threads;
};
#endif

