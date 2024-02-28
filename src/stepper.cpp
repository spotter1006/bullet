#include "stepper.hpp"
#include "defines.hpp"
#include <gpiod.hpp>
#include <thread>
#include <iostream>
#include <signal.h>
#include <thread>
#include <sys/time.h>
#include <unistd.h>
using namespace std;


Stepper::Stepper(){
    m_nPosition = 0;
    gpiod::chip ioChip = gpiod::chip("gpiochip0");

    m_lineEnable =  ioChip.get_line(DRV8825_EN_GPIO);   
    m_lineM0 =      ioChip.get_line(DRV8825_M0_GPIO);
    m_lineM1 =      ioChip.get_line(DRV8825_M1_GPIO);
    m_lineM2 =      ioChip.get_line(DRV8825_M2_GPIO);
    m_lineReset =   ioChip.get_line(DRV8825_RESET_GPIO);
    m_lineSleep =   ioChip.get_line(DRV8825_SLEEP_GPIO);
    m_lineStep =    ioChip.get_line(DRV8825_STEP_GPIO);
    m_lineDir =     ioChip.get_line(DRV8825_DIR_GPIO);
 
    m_lineEnable.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineM0.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineM1.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineM2.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineReset.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineSleep.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineStep.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineDir.request({"bullet",gpiod::line_request::DIRECTION_OUTPUT , 0},0);

    m_lineSleep.set_value(1);
    m_lineEnable.set_value(0);
    m_lineReset.set_value(1);
    
    // 1/32 step
    m_lineM0.set_value(1);
    m_lineM1.set_value(0);
    m_lineM2.set_value(1);

    m_nLeftSweepLimit = 0;
    m_nRightSweepLimit = 0;
    m_nTargetAngle = 0;
    m_fKeepSweeping = true;

}

Stepper::~Stepper(){
    m_lineEnable.release();
    m_lineM0.release();
    m_lineM1.release();
    m_lineM2.release();
    m_lineReset.release();
    m_lineSleep.release();
    m_lineStep.release();
    m_lineDir.release();
}

// 1 for positive direction, -1 for negative. 1 step.
void Stepper::step(int dir){
    m_lineDir.set_value(dir > 0);
    m_lineStep.set_value(1);
    this_thread::sleep_for(chrono::microseconds(2));
    m_lineStep.set_value(0);
    m_nPosition += dir;
}

atomic_flag fWaitForTick;

void Stepper::startSweeping(int left, int right, int stepIntervalUs){
    m_nLeftSweepLimit = left;
    m_nRightSweepLimit = right;
    signal(SIGALRM, [](int signo){fWaitForTick.clear();});   
        
        
    itimerval timer;
    timer.it_interval.tv_usec = stepIntervalUs;      
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = stepIntervalUs;
    timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    
    m_fKeepSweeping=true;

    thread t2([](Stepper* pStepper){
        int dir = 1;
        while(1){
            pStepper->step(dir);

            if(pStepper->getPosition() == pStepper->getRightSweepLimit())  dir = -1;
            else if(pStepper->getPosition() == pStepper->getLeftSweepLimit())  dir = 1;
    
            while(fWaitForTick.test_and_set()) usleep(2);
            
            if(!pStepper->isKeepSweeping() && 
                pStepper->getPosition() == pStepper->getTargetAngle()){
                pStepper->setKeepSweeping(true);
                break;
            }
            
        }
    }, this);
    t2.detach();
}
void Stepper::stopSweeping(int atAngle){
    m_nTargetAngle = atAngle;
    m_fKeepSweeping = false; 
    while(!m_fKeepSweeping){
        usleep(2);
    }

}
