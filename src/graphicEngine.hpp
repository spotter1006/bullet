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
        GraphicEngine(Frame* pFrame, Polar &polarDisplay):
            m_axialPattern(polarDisplay.getLeftSweepLimit(), polarDisplay.getRightSweepLimit(), polarDisplay.getRadius()),
            m_radialPattern(polarDisplay.getLeftSweepLimit(), polarDisplay.getRightSweepLimit(), polarDisplay.getRadius()),
            m_fKeepRunning(true),
            m_pFrame(pFrame){};
        void start();
        void stop();
        void paint();
        inline bool isKeepRunning(){return m_fKeepRunning;}   
        inline void setFrame(Frame *pFrame){m_pFrame = pFrame;}
        inline void setAxialPattern(int center, vector<ws2811_led_t> &leds){m_axialPattern.setPattern(center, leds);}
        inline void setRadialPattern(int center, vector<ws2811_led_t> &leds){m_radialPattern.setPattern(center, leds);}
    private:
        AxialPattern m_axialPattern;
        RadialPattern m_radialPattern;
        Frame *m_pFrame; 
        bool m_fKeepRunning;
        vector<thread> m_threads;
};

#endif