#ifndef _RAY_H_
#define _RAY_H_

#include "graphicElement.hpp"
class Ray: public GraphicElement{
    Ray(int radius,  int sweepAngle) : GraphicElement(GRAPHIC_ELEMENT_RAY,radius,sweepAngle){

    };

};

#endif