#include "axialPattern.hpp"

void AxialPattern::paint(Frame *frame){
    for(int step = m_nMin; step < m_nMax; step++){
        ws2811_led_t pixel  = m_pixels[step];

        if(m_nMin < step && step <= m_nMax){
            frame->setPixel(step, m_nRadius, pixel);
        }
    }
}
void AxialPattern::setPattern(int center, vector<ws2811_led_t> &pattern){
    for(int i = center - pattern.size() /2; i < center + pattern.size() /2; i++){
        m_pixels[i] = pattern[i];
    }
}