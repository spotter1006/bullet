#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>
#include "frame.hpp"
#include <list>

class Polar{
    public:
        Polar();
        ~Polar();
        void start();
        void stop();
        void step(int dir);
        int  getPosition();
        void setBrightness(int val);
        void setMotorStepInterval(int microseconds);
    private:
    
        static void sweeper(Polar* pPolar);
        Stepper m_stepper;
        ws2811_t m_ledstring;
        list<Frame> m_videoBuffer;
};
#endif

