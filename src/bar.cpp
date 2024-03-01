#include "bar.hpp"
#include "defines.hpp"
#include "string.h"
#include <chrono>

Bar::Bar(){
    Bar(LED_STRING_PIXELS, 0);
}
Bar::Bar(int pixels, ws2811_led_t value){
    m_nPixels = pixels;
    for(int i = 0; i < pixels; i++){
        m_pixels[i] = value;
    }
}
void Bar::clear(){
    for(int i = 0; i < m_nPixels; i++){
        m_pixels[i] = BLACK;
    }
}
void Bar::fill(ws2811_led_t value){
     for(int i = 0; i < LED_STRING_PIXELS; i++){
        m_pixels[i] = value;
    }
}
void Bar::setPixel(int i, ws2811_led_t val){
    m_pixels[i] = val;
}

void Bar::render(ws2811_t *ledString){
    memcpy(ledString->channel[0].leds, m_pixels, 10 * sizeof(ws2811_led_t));  
    ws2811_render(ledString);
}