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
    
    // 1/4 step
    m_lineM0.set_value(0);
    m_lineM1.set_value(1);
    m_lineM2.set_value(0);

    thread t1(indexer, this);
    t1.detach();
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
void sig_handler(int signo){
    fWaitForTick.clear();
}

void Stepper::indexer(Stepper* pStepper){

    signal(SIGALRM, sig_handler);
    itimerval timer;
 
    timer.it_interval.tv_usec = 800;      
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = 800;
    timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);


    // Sweep back and forth 60 steps 
    int ticksBetweenSteps = 40;
    int interval = 0;

    int dir = 1;
    while(1){
        if(interval % ticksBetweenSteps == 0)
            pStepper->step(dir);

        if(dir > 0 && ticksBetweenSteps > 1)
            ticksBetweenSteps--;                // Accelerate
        if(dir > 0 && ticksBetweenSteps < 40)
            ticksBetweenSteps++;                // Deccelerate

        if(pStepper->getPosition() == 60)
            dir = -1;
        if(pStepper->getPosition() == 0){
            dir = 1;
            interval = 0;
        }          

        while(fWaitForTick.test_and_set()){
            
            usleep(1);
        }
        

    }
}


