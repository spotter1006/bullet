#include "ledstring.hpp"
#include "defines.hpp"
#include <string.h>

Ledstring::Ledstring(int pixels){
    m_pData = new ws2811_led_t[pixels];

    m_ctx.freq = LED_STRING_FREQUENCY;
    m_ctx.dmanum=10;

    m_ctx.channel[0].gpionum = WSS2812_DATA_GPIO;
    m_ctx.channel[0].invert = 0;
    m_ctx.channel[0].count = pixels;
    m_ctx.channel[0].strip_type = WS2811_STRIP_GBR;
    m_ctx.channel[0].brightness = 255;

    m_ctx.channel[1].gpionum = 0;
    m_ctx.channel[1].invert = 0;
    m_ctx.channel[1].count = 0;
    m_ctx.channel[1].strip_type = 0;
    m_ctx.channel[1].brightness = 0;
    ws2811_init(&m_ctx);

    m_ctx.channel[0].leds = m_pData;    
}

Ledstring::~Ledstring(){
    ws2811_fini(&m_ctx);
    delete m_pData;
}

void Ledstring::clear(){
    memset(m_ctx.channel[0].leds, 0, m_ctx.channel[0].count * sizeof(ws2811_led_t));
}

void Ledstring::render(){
    ws2811_render(&m_ctx);
}

void Ledstring::setPixel(int pixel, ws2811_led_t value){
    m_pData[pixel] = value;
}

void Ledstring::setString(ws2811_led_t *pixels){
    memcpy(m_pData, pixels, m_ctx.channel[0].count);
}

void Ledstring::renderString(ws2811_led_t *data){
    m_ctx.channel[0].leds = data;
    render();
}