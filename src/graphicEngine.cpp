#include "graphicEngine.hpp"
#include "frame.hpp"

using namespace std;

GraphicEngine::GraphicEngine(int frames, int bars, int pixels){
    m_frames = vector<Frame>(frames, Frame(bars, pixels));
}
GraphicEngine::~GraphicEngine(){
    m_frames.clear();
}

vector<Frame>& GraphicEngine::render(){
    int i = 0;
    for(Frame &frame : m_frames){
        for(GraphicElement el : m_elements){
            el.render(i, frame);
        }
        i++;
    }
    return m_frames;
}
