#ifndef _GRPAPIC_ENGINE_H_
#define _GRPAPIC_ENGINE_H_
#include <vector>
#include "frame.hpp"
#include <thread>
#include "axialPattern.hpp"
#include "radialPattern.hpp"

#include "polar.hpp"

using namespace std;

class GraphicEngine{
    public:
        GraphicEngine(Frame* pFrame, Polar &polarDisplay);
        void start();
        void stop();
        void paint();
        inline bool isKeepRunning(){return m_fKeepRunning;}   
        inline void setFrame(Frame *pFrame){m_pFrame = pFrame;}

    private:
        AxialPattern m_directionPattern;
        RadialPattern m_speedPattern;
        Frame *m_pFrame; 
        bool m_fKeepRunning;
        vector<thread> m_threads;
};

#endif