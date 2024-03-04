#ifndef _AXIAL_PATTERN_H_
#define _AXIAL_PATTERN_H_
#include <vector>
#include <ws2811/ws2811.h>
#include "frame.hpp"

class AxialPattern{
    public:
        AxialPattern(int min, int max, int radius): m_pixels(radius), m_nMin(min), m_nMax(max), m_nRadius(radius){}
        void paint(Frame *frame);
    private:
        vector<ws2811_led_t> m_pixels;
        int m_nMin;
        int m_nMax;
        int m_nRadius;
};
#endif