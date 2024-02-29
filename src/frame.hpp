#ifndef _FRAME_H_
#define _FRAME_H_
#include "bar.hpp"
#include <vector>

using namespace std;

class Frame{
    public:
        Frame(int nBars);
        ~Frame();
        void setBar(int step, Bar bar);
        Bar getBar(int step);
    private:
        vector<Bar> m_bars;           // index is step angle

};
#endif