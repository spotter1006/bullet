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

// Set internal pattern object from intensity and color vectors
void Chaser::setPattern(vector<uint32_t> intensities, vector<ws2811_led_t> colors){
    m_pattern.clear();
    transform(intensities.begin(), intensities.end(), colors.begin(), back_inserter(m_pattern), 
    [](uint32_t intensity, ws2811_led_t color){

        int red = (color & 0x00ff0000) >> 16;
        int green = (color & 0x00ff00) >> 8;
        int blue = color & 0x000000ff;

        red *= intensity;
        green *= intensity;
        blue  *= intensity;

        red  /= UINT8_MAX;
        green  /= UINT8_MAX;
        blue  /= UINT8_MAX;

        ws2811_led_t ret = red << 16 | green << 8 | blue;
        return ret;
    });
    m_reversePattern = m_pattern;
    reverse(m_reversePattern.begin(), m_reversePattern.end());
}