#ifndef _PATTERN_H_
#define _PATTERN_H_
#include <vector>
#include <ws2811/ws2811.h>
#include "frame.hpp"
using namespace std;
class Pattern{
    public:
        Pattern(int min, int max) : m_pixels(max-min, 0){
            m_nMin = min;
            m_nMax = max;
        }
        inline void setPattern(vector<ws2811_led_t> pattern){m_pixels = pattern;}
        virtual void paint(Frame *frame);

    protected:
        vector<ws2811_led_t> m_pixels;
        int m_nMin;
        int m_nMax;
};
#endif