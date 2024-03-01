#ifndef _RAY_H_
#define _RAY_H_

#include "graphicElement.hpp"
class Ray: public GraphicElement{
    Ray(int radius, int startStep,  int sweepSteps) : GraphicElement(radius, startStep, sweepSteps){ };

};

#endif