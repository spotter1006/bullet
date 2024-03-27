#ifndef _CHASER_H_
#define _CHASER_H_
#include "defines.hpp"  
#include <vector>
#include <thread>
#include <ws2811/ws2811.h>
using namespace std;

const ws2811_led_t colorGradient[] = {                       // https://colordesigner.io/gradient-generator
    0x00fe0101, 0x00fe2302, 0x00fe4503, 0x00fe6704,     // green to red
    0x00fe8805, 0x00feaa06, 0x00fecb07, 0x00feec08,
    0x00f1ff08, 0x00d1ff09, 0x00b1ff0a, 0x0091ff0b,
    0x0071ff0c, 0x0052ff0d, 0x0033ff0e, 0x0014ff0f
};

class Chaser{
    public:
        Chaser():
        m_pattern(LED_STRING_PIXELS, BLACK),
        m_reversePattern(LED_STRING_PIXELS, BLACK),
        m_ledstring(),
        m_nInterval(100),
        m_intensities({0, 0, 3, 7, 27, 35, 58, 89, 129, 180}),      // Gamma corrected ramp
        m_colors(LED_STRING_PIXELS, BLUE)
        {
            setPattern(m_intensities, m_colors);
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
        void start();
        void stop();
        void rotate(int direction);
        void setPattern(vector<uint32_t> intensities, vector<ws2811_led_t> colors);
        inline bool isKeepRunning(){return m_fKeepRunning;}
        inline void setInterval(int interval){m_nInterval = interval;}
        inline int getInterval(){return m_nInterval;}
        void setIntensities(vector<ws2811_led_t> intensities);
        void setHue(int hue);
        inline ws2811_led_t redToGreen(int val){return colorGradient[(val + 128) / 16];}
    private:
        vector<ws2811_led_t> m_pattern;
        vector<ws2811_led_t> m_reversePattern;
        ws2811_t m_ledstring;
        thread m_thread;
        bool m_fKeepRunning;
        int m_nInterval;            // Interval between pattern shifts of animation in units of calls to chaser.rotate(). Bipolar, aslo sets rotate direction
        vector<ws2811_led_t> m_intensities;
        vector<ws2811_led_t> m_colors;

};
#endif