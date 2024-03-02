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
void GraphicEngine::render(int timeUs){
    for(GraphicElement element : m_elements){
        element.render(timeUs);
    }
}

void GraphicEngine::start(int intervalUs){

    m_fKeepRunning = true;

    m_threads.emplace_back(thread([](GraphicEngine *pGraphicEngine){
        int timeUs = 0;
        while(pGraphicEngine->isKeepRunning()){
            // while(fWait.test_and_set()) usleep(2);     // Wait for interval
            // pGraphicEngine->render(timeUs, frame);
            usleep(200);
        }
    
    },this));
}
void GraphicEngine::stop(){
    m_fKeepRunning = false;
    for(auto& th : m_threads){
        th.join();
    }
}