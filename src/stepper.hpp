#ifndef _STEPPER_H
#define _STEPPER_H
#include <gpiod.hpp>
using namespace std;
extern gpiod::chip chip;

class Stepper{
    public:
        Stepper();
        ~Stepper();
        void step(int dir);
        void startSweeping(int left, int right, int stepIntevalUs);
        void stopSweeping(int atAngle);
        inline int getPosition(){return m_nPosition;}
        inline int getLeftSweepLimit(){return m_nLeftSweepLimit;}
        inline int getRightSweepLimit(){return m_nRightSweepLimit;}
    private:
        int m_nPosition;
        int m_nLeftSweepLimit;
        int m_nRightSweepLimit;
        gpiod::line m_lineEnable;
        gpiod::line m_lineM0;
        gpiod::line m_lineM1;
        gpiod::line m_lineM2;
        gpiod::line m_lineReset;
        gpiod::line m_lineSleep;
        gpiod::line m_lineStep;
        gpiod::line m_lineDir;
        static void sweeper(Stepper* pStepper);
};
#endif