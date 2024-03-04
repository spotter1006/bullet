#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>
#include "frame.hpp"
#include <vector>
#include <thread>

class Polar{
    public:
        Polar(int left, int right, int radius, Frame* pFrame);
        ~Polar();

        void start();
        void stop();

        inline ws2811_t* getLedString(){return &m_ledstring;}
        inline bool isKeepSweeping(){return m_fKeepSweeping;}
        inline void setKeepSweeping(bool val){m_fKeepSweeping=val;}
        inline int getLeftSweepLimit(){return m_nLeftSweepLimit;}
        inline int getRightSweepLimit(){return m_nRightSweepLimit;}
        inline int step(int dir){return m_stepper.step(dir);}
        inline int getStep(){return m_stepper.getPosition();}
        inline void copyBarData(ws2811_led_t* destBar, int i){m_pFrame->copyBarData(destBar, i);}
        inline int getRadius(){return m_nRadius;}

    private:
        Stepper m_stepper;
        ws2811_t m_ledstring;
        Frame *m_pFrame;
        bool m_fKeepSweeping;
        int m_nLeftSweepLimit;
        int m_nRightSweepLimit;
        int m_nRadius;

        vector<thread> m_threads;
};
#endif

