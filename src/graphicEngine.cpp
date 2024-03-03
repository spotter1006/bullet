#include "graphicEngine.hpp"
#include "frame.hpp"
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include "axialPattern.hpp"
#include "radialPattern.hpp"

using namespace std;

GraphicEngine::GraphicEngine(Frame* pFrame, Polar &polarDisplay):
    m_speedPattern(polarDisplay.getLeftSweepLimit(), polarDisplay.getRightSweepLimit(), polarDisplay.getRadius()),
    m_directionPattern(polarDisplay.getLeftSweepLimit(), polarDisplay.getRightSweepLimit(), polarDisplay.getRadius())
{
    m_fKeepRunning = true;
    m_pFrame = pFrame;
    int min = polarDisplay.getLeftSweepLimit();
    int max = polarDisplay.getRightSweepLimit();
    int nPixels = polarDisplay.getRadius();

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
    m_directionPattern.paint(m_pFrame);
    m_speedPattern.paint(m_pFrame);
}