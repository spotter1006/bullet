#include "strobe.hpp"

void Strobe::shutter(ws2811_t *pLedString, int frame, int dir){

    if(frame % m_nPeriod == 0)
         pLedString->channel[0].brightness = 255;
    else if((frame + m_nAperature) % m_nPeriod == 0)
        pLedString->channel[0].brightness = 255;
    
}