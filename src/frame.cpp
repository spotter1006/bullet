#include  "frame.hpp"
#include "defines.hpp"
using namespace std;
        mutex m_mutex;    
void Frame::setBar(int step, Bar bar){   
    m_mutex.lock();
    m_bars[step] = bar;
    m_mutex.unlock();
}
void Frame::getBar(int step, Bar& bar){ 
    m_mutex.lock();
    bar = m_bars[step];
    m_mutex.unlock();
}
void Frame::setPixel(int step, int radius, ws2811_led_t value){ 
    Bar bar;
    getBar(step, bar);
    bar.setPixel(radius, value);
    setBar(step, bar);
}