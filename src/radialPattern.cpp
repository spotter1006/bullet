#include "radialPattern.hpp"

void RadialPattern::render(int step, Frame *frame){
    Bar blankBar = Bar();
    Bar patternBar = Bar(m_pixels);
    
    if(m_nMin < step && step <= m_nMax){
        frame->setBar(step, patternBar);
    }else{
        frame->setBar(step, blankBar);
    }
}