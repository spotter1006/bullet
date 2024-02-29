#include "graphicElement.hpp"

GraphicElement::GraphicElement(){
    m_nRadius = 0;
    m_nSweepAngle = 0;
}

GraphicElement::GraphicElement(int radius, int sweepAngle){

    m_nRadius = radius;
    m_nSweepAngle = sweepAngle;
}

GraphicElement::~GraphicElement(){
    
}
