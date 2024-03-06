#include "graphicEngine.hpp"
#include "frame.hpp"
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include "axialPattern.hpp"
#include "radialPattern.hpp"
#include "defines.hpp"

using namespace std;

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
    //m_radialPattern.paint(m_pFrame);
    //m_axialPattern.paint(m_pFrame);
    m_curvePattern.paint(m_pFrame);

}