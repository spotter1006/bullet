#include "bar.hpp"

Bar::Bar(int pixels){
    m_nPixels = pixels;
    m_pixels = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}
Bar::Bar(Bar& other){
    m_nPixels = other.m_nPixels;
    m_pixels = other.m_pixels;
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