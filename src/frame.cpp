#include  "frame.hpp"

Frame::Frame(){
}

Frame::~Frame(){
    m_bars.empty();
}

void Frame::addBar(int step, Bar bar){
    m_bars[step] = bar;
}
