#ifndef _CHASER_H_
#define _CHASER_H_
#include "defines.hpp"  
#include <vector>
#include <ws2811/ws2811.h>
using namespace std;

class Chaser{
    public:
        Chaser(int leds):
        m_pattern(leds, BLACK),
        m_reversePattern(leds, BLACK),
        m_ledstring()
        {
            m_ledstring.freq = LED_STRING_FREQUENCY;
            m_ledstring.dmanum =10;
            m_ledstring.channel[0].gpionum = WSS2812_DATA_GPIO;
            m_ledstring.channel[0].invert = 0;
            m_ledstring.channel[0].count = LED_STRING_PIXELS;
            m_ledstring.channel[0].strip_type = WS2811_STRIP_GRB;
            m_ledstring.channel[0].brightness = 255;
            m_ledstring.channel[1].gpionum = 0;
            m_ledstring.channel[1].invert = 0;
            m_ledstring.channel[1].count = 0;
            m_ledstring.channel[1].strip_type = 0;
            m_ledstring.channel[1].brightness = 0;
            ws2811_init(&m_ledstring);
        }
        ~Chaser(){
            m_ledstring.channel[0].brightness = 0;
            ws2811_render(&m_ledstring);
            ws2811_fini(&m_ledstring);
        }
        void rotate(int direction);
        void setPattern(vector<uint32_t> intensities, vector<ws2811_led_t> colors);
    private:
        vector<ws2811_led_t> m_pattern;
        vector<ws2811_led_t> m_reversePattern;
        ws2811_t m_ledstring;
};
#endif