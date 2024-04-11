#ifndef _BARGRAPH_H_
#define _BARGRAPH_H_
#include <ws2811/ws2811.h>
#include <vector>
#include "defines.hpp"
#include <iostream>

using namespace std;

class Bargraph{
    public: 
        Bargraph():
         m_barPattern{            // https://colordesigner.io/gradient-generator
                0x00ff0000,
                0x00fc4300,
                0x00f66300,
                0x00ea7f00,
                0x00dd9600,
                0x00ceaa00,
                0x00bdbc00,
                0x00a9ce00,
                0x008fdf00,
                0x0065f000,
                0x0005ff0d,
                BLUE
        },

        m_pattern(LED_STRING_PIXELS, BLACK)
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
            ws2811_return_t result = ws2811_init(&m_ledstring);
            if(result != WS2811_SUCCESS) cout << "ws2811 init return error: " << result << endl;
           
        }
        ~Bargraph(){
            m_ledstring.channel[0].brightness = 0;
            ws2811_render(&m_ledstring);
            ws2811_fini(&m_ledstring);
        }
        void setValue(int value);
    private:
        ws2811_t m_ledstring;
        vector<ws2811_led_t> m_pattern;
        vector<ws2811_led_t> m_barPattern;
};

#endif