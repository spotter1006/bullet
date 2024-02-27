#ifndef _POLAR_H_
#define _POLAR_H_

#include "stepper.hpp"
#include <ws2811/ws2811.h>

class Polar{
    public:
        Polar();
        ~Polar();
        void start();
        void stop();
        void step(int dir);
        int  getPosition();
        void render();
        void blankString();
        void setBrightness(int val);
        void setPixel(int pixel, int value);
    private:
        static void sweeper(Polar* pPolar);
        Stepper *m_pStepper;
        ws2811_t *m_pLedstring;
};
#endif
