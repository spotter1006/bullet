#pragma once
#include <vector>
#include <ws2811/ws2811.h>
using namespace std;
class Bar{
    public:
        Bar(int pixels);
        Bar(Bar& other);
        ~Bar();
        void clear();
        void fill(ws2811_led_t val);
        void setPixel(int i, ws2811_led_t val);
    private:
        int m_nPixels;
        vector<ws2811_led_t> m_pixels;
};