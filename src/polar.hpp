#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>
#include "frame.hpp"
#include <vector>

class Polar{
    public:
        Polar();
        ~Polar();
        void start();
        void stop();
        void step(int dir);
        int getPosition();
        int getDirection();
        int getRightSweepLimit();
        int getLeftSweepLimit();
        void setBrightness(int val);
        inline ws2811_t* getLedString(){return &m_ledstring;}
        inline bool isKeepRunning(){return m_fKeepRunning;}
        inline void setKeepRunning(bool val){m_fKeepRunning=val;}
        inline int getFrameCount(){return m_frames.size();}
        int waitForNextStep();
        Frame getFrame(int step){return m_frames[step];}
    private:
        Stepper m_stepper;
        ws2811_t m_ledstring;
        vector<Frame> m_frames;
        bool m_fKeepRunning;
};
#endif

