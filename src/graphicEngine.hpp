#ifndef _GRPAPIC_ENGINE_H_
#define _GRPAPIC_ENGINE_H_
#include <vector>
#include "graphicElement.hpp"
#include "frame.hpp"
#include <thread>

using namespace std;

class GraphicEngine{
    public:
        GraphicEngine(int nFrames, int bars, int pixels);
        ~GraphicEngine();
        void start();
        void stop();
        void addElement(GraphicElement element);
        void render(int timeUs); 
        inline bool isKeepRunning(){return m_fKeepRunning;}   
        inline void setFrame(Frame *pFrame){m_pFrame = pFrame;}
    private:
        vector<GraphicElement> m_elements;     
        Frame *m_pFrame; 
        bool m_fKeepRunning;
        vector<thread> m_threads;
};

#endif