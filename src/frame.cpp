#include  "frame.hpp"
#include "defines.hpp"
#include <string.h>
using namespace std;
mutex m_mutex;    
void Frame::setBar(int step, vector<ws2811_led_t> bar){   
    m_mutex.lock();
    m_bars[step] = bar;
    m_mutex.unlock();
}
void Frame::copyBarData(ws2811_led_t* destBar, int i){ 
    m_mutex.lock();
    memcpy(destBar, m_bars[i].data(), LED_STRING_PIXELS * sizeof(ws2811_led_t));
    m_mutex.unlock();
}
void Frame::setPixel(int step, int radius, ws2811_led_t value){ 
    m_mutex.lock();
    m_bars[step][radius] = value;
    m_mutex.unlock();
}