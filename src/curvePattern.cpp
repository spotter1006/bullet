#include "curvePattern.hpp"
#include "defines.hpp"
#include <algorithm>

void CurvePattern::paint(Frame *frame){
    int rate = (m_nMax - m_nMin) / m_pixels.size();
    vector<ws2811_led_t> v(m_pixels.size(), BLACK);
    v[0] = GREEN;     // TODO: from m_pixels somehow...
    for(int step = m_nMin; step < m_nMax; step++){
        if(step % rate == 0){
            frame->setBar(step, v);
            rotate(v.rbegin(), v.rbegin() + 1, v.rend());
        }
    }

}
void CurvePattern::setPattern(int center, vector<ws2811_led_t> &pattern){
    for(int i = center - pattern.size() /2; i < center + pattern.size() /2; i++){
        m_pixels[i] = pattern[i];
    }
}