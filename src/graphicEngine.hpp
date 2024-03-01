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
        void start(int intervalUs);
        void stop();
        void addElement(GraphicElement element);
        void render(int step, Frame& frame); 
        inline bool isKeepRunning(){return m_fKeepRunning;}      
    private:
        vector<GraphicElement> m_elements;      
        bool m_fKeepRunning;
        thread m_thread;
};

#endif