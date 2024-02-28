#ifndef _ARC_H_ARC
#define _ARC_H_ARC

#include "graphicElement.hpp"
class Arc: public GraphicElement{
    public:
        Arc(int radius, int sweepAngle) : GraphicElement(GRAPHIC_ELEMENT_ARC, radius, sweepAngle){};

    private:
};
#endif