#include "graphicElement.hpp"

GraphicElement::GraphicElement(){
    m_nType = GRAPHIC_ELEMENT_RAY;
    m_nRadius = 0;
    m_nSweepAngle = 0;
}

GraphicElement::GraphicElement( GRAPHIC_ELEMENT type, int radius, int sweepAngle){
    m_nType = type;
    m_nRadius = radius;
    m_nSweepAngle = sweepAngle;
}

GraphicElement::~GraphicElement(){
    
}
