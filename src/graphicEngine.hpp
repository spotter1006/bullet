#ifndef _GRPAPIC_ENGINE_H_
#define _GRPAPIC_ENGINE_H_
#include <vector>
#include "frame.hpp"
#include <thread>
#include "axialPattern.hpp"
#include "radialPattern.hpp"
#include "curvePattern.hpp"
#include "polar.hpp"
#include "motionSimulator.hpp"
#include "defines.hpp"

using namespace std;

class GraphicEngine{
    public:
        GraphicEngine(Frame* pFrame, Polar &polarDisplay):
            m_axialPattern(polarDisplay.getLeftSweepLimit(), polarDisplay.getRightSweepLimit(), polarDisplay.getRadius()),
            m_radialPattern(polarDisplay.getLeftSweepLimit(), polarDisplay.getRightSweepLimit(), polarDisplay.getRadius()),
            m_curvePattern(polarDisplay.getLeftSweepLimit(), polarDisplay.getRightSweepLimit(), polarDisplay.getRadius()),
            m_fKeepRunning(true),
            m_pFrame(pFrame),
            m_simulator(0, 5.0, 0.1, 0.001, MOTOR_SWEEP_STEPS, LED_STRING_PIXELS-1, 10, 1)         // Test 
            {};
        void start();
        void stop();
        void paint();
        inline bool isKeepRunning(){return m_fKeepRunning;}   
        inline void setFrame(Frame *pFrame){m_pFrame = pFrame;}
        inline void setAxialPattern(int center, vector<ws2811_led_t> &leds){m_axialPattern.setPattern(center, leds);}
        inline void setRadialPattern(int center, vector<ws2811_led_t> &leds){m_radialPattern.setPattern(center, leds);}
        inline void setCurvePattern(int center, vector<ws2811_led_t> &leds){m_radialPattern.setPattern(center, leds);}

        inline void simulatorStep(){m_simulator.step();}
        inline vector<int>& getAngleHistogram(){return m_simulator.getAngleHistogram();}
        inline vector<int>& getMagnitudeHistogram(){return m_simulator.getMagnitudeHistogram();}
    private:
        AxialPattern m_axialPattern;
        RadialPattern m_radialPattern;
        CurvePattern m_curvePattern;

        Frame *m_pFrame; 
        bool m_fKeepRunning;
        vector<thread> m_threads;
        MotionSimulator m_simulator;        // Test
};

#endif