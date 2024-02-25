#ifndef _ledstring_h_
#define _ledstring_h_

#include <ws2811/ws2811.h>

class Ledstring{
    public:
        Ledstring(int pixels);
        ~Ledstring();
        void setPixel(int pixel, ws2811_led_t value);
        void render(void);
    private:
        ws2811_t m_ctx;
        ws2811_led_t *m_pData;

};
#endif