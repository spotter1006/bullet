#include "axialPattern.hpp"

void AxialPattern::paint(Frame *frame){
    for(int step = m_nCenter - m_pixels.size() / 2; step < m_nCenter + m_pixels.size() / 2; step++){
        int i = step - m_nCenter + m_pixels.size() / 2;
        frame->setPixel(step, m_nRadius - 1, m_pixels[i]);
    }

}
void AxialPattern::setPattern(int center, vector<ws2811_led_t> &pattern){
    for(int i = center - pattern.size() /2; i < center + pattern.size() /2; i++){
        m_pixels[i] = pattern[i];
    }
}