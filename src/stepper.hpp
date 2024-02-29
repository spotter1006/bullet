#ifndef _STEPPER_H
#define _STEPPER_H
#include <gpiod.hpp>
using namespace std;
extern gpiod::chip chip;

class Stepper{
    public:
        Stepper();
        ~Stepper();
        int step(int dir);
        inline int getDirection(){return (m_lineDir.get_value() == 1)? 1 : -1;}
        inline int getPosition(){return m_nPosition;}
        int m_nPosition;
        gpiod::line m_lineEnable;
        gpiod::line m_lineM0;
        gpiod::line m_lineM1;
        gpiod::line m_lineM2;
        gpiod::line m_lineReset;
        gpiod::line m_lineSleep;
        gpiod::line m_lineStep;
        gpiod::line m_lineDir;

};
#endif