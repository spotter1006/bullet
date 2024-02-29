#pragma once
#include <vector>
#include <ws2811/ws2811.h>
using namespace std;
class Bar{
    public:
        Bar(int pixels, ws2811_led_t value);
        ~Bar();
        void clear();
        void fill(ws2811_led_t val);
        void setPixel(int i, ws2811_led_t val);
        void render(ws2811_t *ledString);
    private:
        vector<ws2811_led_t> m_pixels;
};