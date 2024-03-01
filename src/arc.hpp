#ifndef _ARC_H_ARC
#define _ARC_H_ARC

#include "graphicElement.hpp"
class Arc: public GraphicElement{
    public:
        Arc(int radius, int startStep,  int sweepSteps) : GraphicElement(radius, startStep, sweepSteps){
            
        };
    private:
};
#endif