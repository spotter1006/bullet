#include "graphicEngine.hpp"
#include "frame.hpp"
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>


using namespace std;

GraphicEngine::GraphicEngine(int frames, int bars, int pixels){
    m_fKeepRunning = true;
}
GraphicEngine::~GraphicEngine(){

}
void GraphicEngine::addElement(GraphicElement element){
    m_elements.push_back(element);
}
void GraphicEngine::render(int step, Frame &frame){
    for(GraphicElement el : m_elements){
        el.render(step, frame);
    }
}
atomic_flag fWait;
void GraphicEngine::start(int intervalUs){
    signal(SIGALRM, [](int signo){fWait.clear();});   
    itimerval timer;
    timer.it_interval.tv_usec = intervalUs;      
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = intervalUs;
    timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    m_thread = thread([](GraphicEngine *pGraphicEngine){
        while(pGraphicEngine->isKeepRunning()){
            while(fWait.test_and_set()){usleep(2);}
            // render for tick
            // set frame in target
        }
        
    },this);
}
void GraphicEngine::stop(){
    m_fKeepRunning = false;
    m_thread.join();
}