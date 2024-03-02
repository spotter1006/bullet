#ifndef _BAR_H_
#define _BAR_H_

#include <vector>
#include <ws2811/ws2811.h>
#include "defines.hpp"


using namespace std;
class Bar{
    public:
        Bar();
        Bar(vector<ws2811_led_t> &pixels);
        Bar(int pixels, ws2811_led_t value);
        void clear();
        void fill(ws2811_led_t val);
        void setPixel(int i, ws2811_led_t val);
        void display(ws2811_t *ledString);
    private:
        int m_nPixels;
        ws2811_led_t m_pixels[LED_STRING_PIXELS];
 
};
#endif