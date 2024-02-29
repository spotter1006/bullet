#include "bar.hpp"


Bar::Bar(int pixels, ws2811_led_t value){
    m_pixels = vector<ws2811_led_t>(pixels, value);
}

Bar::~Bar(){
    m_pixels.empty();
}   
void Bar::clear(){
    m_pixels = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}
void Bar::fill(ws2811_led_t val){
    m_pixels = {val, val, val, val, val, val,val, val,val, val};
}
void Bar::setPixel(int i, ws2811_led_t val){
    m_pixels[i] = val;
}
void Bar::render(ws2811_t *ledString){
    for(int i = 0; i < m_pixels.size(); i++){
        ledString->channel[0].leds[i] = m_pixels[i];
    }
    ws2811_render(ledString);
}