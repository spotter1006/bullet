#ifndef _STEPPER_H
#define _STEPPER_H
#include <gpiod.hpp>
using namespace std;
extern gpiod::chip chip;

class Stepper{
    public:
        Stepper();
        ~Stepper();
        int step();
        void setMode(int mode);
        inline int getMode(){return m_nMode;}
        inline int getPosistion(){return m_nPosition;}
    private:
        int m_nPosition;
        int m_nMode;
        line m_lineEnable;
        line m_lineM0;
        line m_lineM1;
        line m_lineM2;
        line m_lineReset;
        line m_lineSleep;
        line m_lineStep;
        line m_lineDir;


};
#endif