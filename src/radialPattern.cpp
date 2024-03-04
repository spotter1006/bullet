#include "radialPattern.hpp"

void RadialPattern::paint(Frame *frame) {
    for(int step = m_nMin; step < m_nMax; step++){
        vector<ws2811_led_t> blankBar = vector<ws2811_led_t>();
        vector<ws2811_led_t> patternBar = vector<ws2811_led_t>(m_pixels);

        if(m_nMin < step && step <= m_nMax){
            frame->setBar(step, patternBar);
        }else{
            frame->setBar(step, blankBar);
        }
 
    }
}