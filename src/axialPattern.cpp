#include "axialPattern.hpp"

void AxialPattern::paint(Frame *frame){
    for(int step = m_nMin; step < m_nMax; step++){
        ws2811_led_t pixel  = m_pixels[step];
        Bar bar = Bar();
        bar.setPixel(m_nRadius, pixel);
        if(m_nMin < step && step <= m_nMax){
            frame->setPixel(step, m_nRadius, pixel);
        }
    }
}