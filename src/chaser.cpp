#include "chaser.hpp"
#include <algorithm>
#include <string.h>

using namespace std;

void Chaser::rotate(int direction){
    switch(direction){
        case 1:
            std::rotate(m_pattern.rbegin(), m_pattern.rbegin() + 1, m_pattern.rend());
            break;
        case -1: 
            std::rotate(m_pattern.begin(), m_pattern.begin() + 1, m_pattern.end());
            break;
        default:
            break;

    };
    memcpy(m_ledstring.channel[0].leds, m_pattern.data(), LED_STRING_PIXELS * sizeof(ws2811_led_t) );
    ws2811_render(&m_ledstring);
}

