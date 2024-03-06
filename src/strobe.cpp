#include "strobe.hpp"

void Strobe::shutter(ws2811_t *pLedString, int interval, int dir){

    if(interval % m_nPeriod == 0 && dir == m_nDirection)
        pLedString->channel[0].brightness = 255;
    if(interval  % (m_nPeriod + m_nAperature) == 0)
        pLedString->channel[0].brightness = 0;
    
}