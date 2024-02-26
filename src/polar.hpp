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
        int getPosition();
        void render(ws2811_led_t *data);
        void setPattern(ws2811_led_t *pattern, int sizeInPixels);
        inline void setPatternIndex(int index){m_nPatternIndex = index;}
        void displayNextPatternFrame();
        void blankString();
    private:
        static void sweeper(Polar* pPolar);
        Stepper *m_pStepper;
        ws2811_t *m_pLedstring;
        ws2811_led_t *m_pPattern;      
        int m_nPatternIndex;
        int m_nPatternSize;
};
#endif