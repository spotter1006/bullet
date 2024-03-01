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

        void addElement(GraphicElement element);
        void render(int step, Frame& frame);       
    private:
        vector<GraphicElement> m_elements;      // Inputs to engine
        vector<Frame> m_frames;                 // Output of engine
};

#endif