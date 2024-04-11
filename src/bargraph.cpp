#include "bargraph.hpp"
#include <math.h>
#include <string.h>

void Bargraph::setValue(int value){
    m_pattern = {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

    int count = abs(value);
    if(count > BARGRAPH_ZERO_PIXEL) count = BARGRAPH_ZERO_PIXEL;
    int i = BARGRAPH_ZERO_PIXEL;

    if(value > 0){
        while(count){
            // m_pattern[i] = m_barPattern[i];
            m_pattern[i] = GREEN;
            i++;
            count--;
        }
    }else if(value < 0){
        while(count){
            // m_pattern[i] = m_barPattern[i];
            m_pattern[i] = RED;
            i--;
            count--;
        }
    }
    
    m_pattern[LED_STRING_PIXELS - 1] = WHITE;  // Last LED lights the heading display
    
    memcpy(m_ledstring.channel[0].leds, m_pattern.data(), LED_STRING_PIXELS * sizeof(ws2811_led_t));
    ws2811_render(&m_ledstring);
}