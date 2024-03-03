#ifndef _RADIAL_PATTERN_H_
#define _RADIAL_PATTERN_H_
#include "pattern.hpp"
class RadialPattern : public Pattern{
    public:
        RadialPattern(int min, int max, int radius) : Pattern(min,max,radius){};
        void paint(Frame *frame) override;
    private:
};
#endif