#ifndef _DISPLAY_ELEMENT_H_
#define _DISPLAY_ELEMENT_H_

#include <vector>
#include <ws2811/ws2811.h>
#include "defines.hpp"
#include "frame.hpp"

using namespace std;

typedef enum graphic_element{
    GRAPHIC_ELEMENT_RAY,
    GRAPHIC_ELEMENT_ARC
}GRAPHIC_ELEMENT;

class GraphicElement{
    public:
        GraphicElement();
        GraphicElement(int radius, int startStep,  int sweepSteps);
        ~GraphicElement();
        virtual void render(int step, Frame &frame);
    protected:
        int m_nRadius;
        int m_nStartStep;
        int m_nSweepSteps;
        ws2811_led_t m_barPattern[LED_STRING_PIXELS];
};
#endif