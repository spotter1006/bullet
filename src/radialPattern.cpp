#include "radialPattern.hpp"

void RadialPattern::paint(Frame *frame) {
    for(int step = m_nMin; step < m_nMax; step++){
        vector<ws2811_led_t> blankBar = vector<ws2811_led_t>();
        vector<ws2811_led_t> patternBar = vector<ws2811_led_t>(m_pixels);

        if(step == m_nCenter){
            frame->setBar(step, patternBar);
        }else{
            frame->setBar(step, blankBar);
        }
    }
}
void RadialPattern::setPattern(int center, vector<ws2811_led_t> &pattern){
    for(int i = center - pattern.size() /2; i < center + pattern.size() /2; i++){
        m_pixels[i] = pattern[i];
    }
}