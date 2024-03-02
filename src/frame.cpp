#include  "frame.hpp"
#include "defines.hpp"
using namespace std;

mutex barMutex;

Frame::Frame(){
    Frame(MOTOR_SWEEP_STEPS, LED_STRING_PIXELS);
}
Frame::Frame(int nBars, int pixels){
    Bar initBar = Bar(pixels, 0);
    m_bars = vector<Bar>(nBars, initBar);
}
Frame::Frame(const Frame &other){
    m_bars = other.m_bars;
}
Frame::~Frame(){
    m_bars.erase(m_bars.begin(), m_bars.end());
}
void Frame::setBar(int step, Bar bar){    // Thread safe
    barMutex.lock();
    m_bars[step] = bar;
    barMutex.unlock();
}
void Frame::getBar(int step, Bar& bar){ // Thread safe
    barMutex.lock();
    bar = m_bars[step];
    barMutex.unlock();
}
void Frame::setPixel(int step, int radius, ws2811_led_t value){ // Thread safe
    Bar bar;
    getBar(step, bar);
    bar.setPixel(radius, value);
    setBar(step, bar);
}