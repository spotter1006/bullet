#ifndef _FRAME_H_
#define _FRAME_H_
// #include "bar.hpp"
#include <ws2811/ws2811.h>
#include <vector>
#include <mutex>

using namespace std;
class Frame{
    public:
        Frame(int nBars, int pixels) : m_bars(nBars){
            for(int i =0; i < nBars; i++){
                m_bars[i] = vector<ws2811_led_t>(pixels, 0);
            }
        };
        void setBar(int step, vector<ws2811_led_t> bar);
        void copyBarData(ws2811_led_t* destBar, int i);
        void setPixel(int step, int radius, ws2811_led_t value);
    private:
        vector<vector<ws2811_led_t>> m_bars;   
};
#endif