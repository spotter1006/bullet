#include "graphicElement.hpp"
#include "frame.hpp"

GraphicElement::GraphicElement(){
    GraphicElement(0, 0,  0);
}

GraphicElement::GraphicElement(int radius, int startStep,  int sweepSteps){
    m_nRadius = radius;
    m_nSweepSteps = sweepSteps;
    m_nStartStep = startStep;
}

GraphicElement::~GraphicElement(){
    
}
void GraphicElement::render(int step, Frame &frame){
    // Override me
}
