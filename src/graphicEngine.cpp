#include "graphicEngine.hpp"
#include "frame.hpp"

using namespace std;

GraphicEngine::GraphicEngine(int frames, int bars, int pixels){
    m_frames = vector<Frame>(frames, Frame(bars, pixels));
}
GraphicEngine::~GraphicEngine(){
    m_frames.clear();
}
void GraphicEngine::addElement(GraphicElement element){
    m_elements.push_back(element);
}
void GraphicEngine::render(int step, Frame &frame){
    for(GraphicElement el : m_elements){
        el.render(step, frame);
    }
}
