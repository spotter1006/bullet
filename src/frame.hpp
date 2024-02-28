#ifndef _FRAME_H_
#define _FRAME_H_
#include "bar.hpp"
#include <vector>
using namespace std;
class Frame{
    public:
        Frame();
        ~Frame();
        void addBar(int step, Bar bar);
    private:
        vector<Bar> m_bars;           // Key is step angle

};
#endif