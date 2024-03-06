#include "chaser.hpp"
#include <algorithm>
#include <string.h>
#include <algorithm>

using namespace std;

void Chaser::rotate(int direction){
    switch(direction){
        case 1:
            std::rotate(m_pattern.rbegin(), m_pattern.rbegin() + 1, m_pattern.rend());
            memcpy(m_ledstring.channel[0].leds, m_pattern.data(), LED_STRING_PIXELS * sizeof(ws2811_led_t) );
            break;
        case -1: 
            std::rotate(m_reversePattern.begin(), m_reversePattern.begin() + 1, m_reversePattern.end());
            memcpy(m_ledstring.channel[0].leds, m_reversePattern.data(), LED_STRING_PIXELS * sizeof(ws2811_led_t) );
            break;
        default:
            break;

    };
    
    ws2811_render(&m_ledstring);
}

void Chaser::setPattern(vector<ws2811_led_t> &pattern){
    m_reversePattern = m_pattern =  pattern;
    reverse(m_reversePattern.begin(), m_reversePattern.end());
}