#ifndef _STEPPER_H
#define _STEPPER_H
#include <gpiod.hpp>
#include <thread>
using namespace std;
extern gpiod::chip chip;

class Stepper{
    public:
        Stepper();
        ~Stepper();
        int step(int dir);
        inline int getMotorDirection(){return (m_lineDir.get_value() == 1)? 1 : -1;}
        inline int getPosition(){return m_nPosition;}
        inline int getTargetPosition(){return m_nTargetPosition;}
        inline void setTargetPosition(int position){m_nTargetPosition = position;}
        inline void zeroPosition(){m_nPosition = 0;}
        inline void enable(bool enable){m_lineEnable.set_value(enable? 0:1);}
        inline bool isKeepRunning(){return m_fKeepRunning;}
        void resetPulse();
        void start();
        void stop();
    private:
        int m_nPosition;
        int m_nTargetPosition;
        gpiod::line m_lineEnable;
        gpiod::line m_lineM0;
        gpiod::line m_lineM1;
        gpiod::line m_lineM2;
        gpiod::line m_lineReset;
        gpiod::line m_lineSleep;
        gpiod::line m_lineStep;
        gpiod::line m_lineDir;
        thread m_thread;
        bool m_fKeepRunning;

};
#endif