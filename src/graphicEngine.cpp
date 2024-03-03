#include "graphicEngine.hpp"
#include "frame.hpp"
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include "axialPattern.hpp"
#include "radialPattern.hpp"

using namespace std;

GraphicEngine::GraphicEngine(Frame* pFrame, Polar &polarDisplay){
    m_fKeepRunning = true;
    m_pFrame = pFrame;
    int min = polarDisplay.getLeftSweepLimit();
    int max = polarDisplay.getRightSweepLimit();
    int nPixels = polarDisplay.getRadius();
    m_patterns.push_back(AxialPattern(min, max, nPixels));
    m_patterns.push_back(RadialPattern(0, nPixels));
};

void GraphicEngine::start(){
    m_fKeepRunning = true;
    m_threads.emplace_back(thread([](GraphicEngine *pGraphicEngine){
        int timeInterval = 0;
        while(pGraphicEngine->isKeepRunning()){
            auto wake = chrono::steady_clock::now() + chrono::milliseconds(AMIMATION_INTERVAL_MS); 
            pGraphicEngine->paint();
            this_thread::sleep_until(wake);
        }  
    },this));
}
void GraphicEngine::stop(){
    m_fKeepRunning = false;
    for(auto& th : m_threads){
        th.join();
    }
}
void GraphicEngine::paint(){
    for(Pattern &pattern : m_patterns){
        pattern.paint(m_pFrame);
    }
}