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
void GraphicEngine::addElement(GraphicElement element){
    m_elements.push_back(element);
}
void GraphicEngine::render(int timeInterval){
    for(GraphicElement element : m_elements){
        element.render(timeInterval);
    }
}

void GraphicEngine::start(){
    m_fKeepRunning = true;
    m_threads.emplace_back(thread([](GraphicEngine *pGraphicEngine){
        int timeInterval = 0;
        while(pGraphicEngine->isKeepRunning()){
            auto wake = chrono::steady_clock::now() + chrono::milliseconds(AMIMATION_INTERVAL_MS); 
            pGraphicEngine->render(timeInterval);
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