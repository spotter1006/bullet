#ifndef _DISPLAY_ELEMENT_H_
#define _DISPLAY_ELEMENT_H_

#include <vector>
#include <ws2811/ws2811.h>

using namespace std;

typedef enum graphic_el{
    GRAPHIC_ELEMENT_RAY,
    GRAPHIC_ELEMENT_ARC,
    GRAPHIC_ELEMENT_WEDGE

}GRAPHIC_ELEMENT;

class GraphicElement{
    public:
        GraphicElement();
        GraphicElement(int radius,  int sweepAngle);
        ~GraphicElement();
    protected:
        int m_nRadius;
        int m_nSweepAngle;
        vector<ws2811_led_t> m_pixelBuffer;
};
#endif