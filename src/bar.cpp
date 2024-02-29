#include "bar.hpp"
#include "defines.hpp"
#include "string.h"
#include <chrono>

Bar::Bar(int pixels, ws2811_led_t value){
    for(int i = 0; i < LED_STRING_PIXELS; i++){
        m_pixels[i] = value;
    }
}

void Bar::clear(){
    for(int i = 0; i < LED_STRING_PIXELS; i++){
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
    // auto start = std::chrono::high_resolution_clock::now();

    memcpy(ledString->channel[0].leds, m_pixels, 10 * sizeof(ws2811_led_t));  
    ws2811_render(ledString);
    // auto elapsed = std::chrono::high_resolution_clock::now() - start;
    // long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}