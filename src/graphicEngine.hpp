#ifndef _GRPAPIC_ENGINE_H_
#define _GRPAPIC_ENGINE_H_
#include <vector>
#include "graphicElement.hpp"
#include "frame.hpp"

using namespace std;

class GraphicEngine{
    public:
        GraphicEngine(int nFrames, int bars, int pixels);
        ~GraphicEngine();
        vector<Frame>& render();
        inline void addElement(GraphicElement element){m_elements.push_back(element);}
        
        
    private:
        vector<GraphicElement> m_elements;      // Inputs to engine
        vector<Frame> m_frames;                 // Output of engine
};

#endif