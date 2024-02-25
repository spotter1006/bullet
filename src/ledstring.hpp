#ifndef _ledstring_h_
#define _ledstring_h_

#include <ws2811/ws2811.h>

class Ledstring{
    public:
        Ledstring(int pixels);
        ~Ledstring();
        void clear();
        void setPixel(int pixel, ws2811_led_t value);
        void setString(ws2811_led_t *pixels);
        void render(void);
        void renderString(ws2811_led_t *data);
    private:
        ws2811_t m_ctx;
        ws2811_led_t *m_pData;
};
#endif