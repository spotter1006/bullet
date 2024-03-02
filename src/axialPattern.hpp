#ifndef _AXIAL_PATTERN_H_
#define _AXIAL_PATTERN_H_
#include "pattern.hpp"
class AxialPattern : public Pattern {
    public:
        AxialPattern(int min, int max, int radius) : Pattern(min, max){m_nRadius = radius;};
        void render(int step, Frame *frame);
    protected:
        int m_nRadius;
};
#endif