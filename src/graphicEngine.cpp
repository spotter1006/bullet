#include "graphicEngine.hpp"
#include "frame.hpp"
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

GraphicEngine::GraphicEngine(int frames, int bars, int pixels){
    m_fKeepRunning = true;
    m_pFrame = NULL;
}
GraphicEngine::~GraphicEngine(){

}

void GraphicEngine::start(){
    m_fKeepRunning = true;
    m_threads.emplace_back(thread([](GraphicEngine *pGraphicEngine){
        int timeInterval = 0;
        while(pGraphicEngine->isKeepRunning()){
            auto wake = chrono::steady_clock::now() + chrono::milliseconds(AMIMATION_INTERVAL_MS); 
            // Paint all patterns to the frame
            // TODO: put the frame in the polar display
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