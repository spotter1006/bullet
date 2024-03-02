#ifndef _RADIAL_PATTERN_H_
#define _RADIAL_PATTERN_H_
#include "pattern.hpp"
class RadialPattern : public Pattern{
    public:
        RadialPattern(int min, int max) : Pattern(min,max){};
        void paint(int step, Frame *frame);
    private:
};
#endif