#ifndef _CURVE_PATTERN_H
#define _CURVE_PATTERN_H
#include <vector>
#include <ws2811/ws2811.h>
#include "frame.hpp"

class CurvePattern{
    public:
        CurvePattern(int min, int max, int radius): 
        m_pixels(radius), 
        m_nMin(min), 
        m_nMax(max), 
        m_nRadius(radius), 
        m_nCenter((max-min)/2){}

        void paint(Frame *frame);
        void setPattern(int center, vector<ws2811_led_t> &pattern);
    private:
        vector<ws2811_led_t> m_pixels;
        int m_nMin;
        int m_nMax;
        int m_nRadius;
        int m_nCenter;
};
#endif