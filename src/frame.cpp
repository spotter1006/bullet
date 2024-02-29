#include  "frame.hpp"
#include "defines.hpp"

Frame::Frame(int nBars){
    Bar initBar = Bar(LED_STRING_PIXELS, 0);
    m_bars = vector<Bar>(nBars, initBar);

}

Frame::~Frame(){
    m_bars.erase(m_bars.begin(), m_bars.end());
}

void Frame::setBar(int step, Bar bar){
    m_bars[step] = bar;
}

Bar Frame::getBar(int step){
    return m_bars[step];
}