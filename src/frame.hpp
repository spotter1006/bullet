#ifndef _FRAME_H_
#define _FRAME_H_
#include "bar.hpp"
#include <vector>
#include <mutex>

using namespace std;

class Frame{
    public:
        Frame();
        Frame(int nBars, int pixels);
        Frame(const Frame &other);
        ~Frame();
        void setBar(int step, Bar bar);
        void getBar(int step, Bar& bar);
        void setPixel(int step, int radius, ws2811_led_t value);
    private:
        vector<Bar> m_bars;           // index is step angle
  
};
#endif