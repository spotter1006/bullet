#ifndef _STROBE_H_
#define _STROBE_H_
#include <ws2811/ws2811.h>

 class Strobe{
    public:
        Strobe(int period, int aperature, int direction):
            m_nPeriod(period),
            m_nAperature(aperature),
            m_nDirection(direction){}
        void shutter(ws2811_t *pLedString, int frame, int dir);
    private:
        int m_nPeriod;
        int m_nAperature;
        int m_nDirection;

 };
#endif