#ifndef _AXIAL_PATTERN_H_
#define _AXIAL_PATTERN_H_
#include "pattern.hpp"
class AxialPattern : public Pattern {
    public:
        AxialPattern(int min, int max, int radius) : Pattern(min, max, radius){}
        void paint(Frame *frame) override;
    protected:
        
};
#endif