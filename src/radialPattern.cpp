#include "radialPattern.hpp"

void RadialPattern::paint(Frame *frame) {
    for(int step = m_nMin; step < m_nMax; step++){
        Bar blankBar = Bar();
        Bar patternBar = Bar(m_pixels);

        if(m_nMin < step && step <= m_nMax){
            frame->setBar(step, patternBar);
        }else{
            frame->setBar(step, blankBar);
        }
 
    }
}