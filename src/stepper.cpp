#include "stepper.hpp"
#include "defines.hpp"
#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

Stepper::Stepper(){
    m_nPosition = 0;
    m_nTargetPosition = 0;
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
    
    // 1/8 step
    m_lineM0.set_value(1);
    m_lineM1.set_value(1);
    m_lineM2.set_value(0);

    m_fKeepRunning = true;
}

Stepper::~Stepper(){
    m_lineEnable.set_value(1);
    
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
int Stepper::step(int dir){
    m_lineDir.set_value(dir > 0); 
    m_lineStep.set_value(1);
    usleep(2);
    m_lineStep.set_value(0);
    m_nPosition += dir;
    return m_nPosition;
}

void Stepper::resetPulse(){
    m_lineReset.set_value(0);
    usleep(2);
    m_lineReset.set_value(1);
}

void Stepper::start(){
    m_fKeepRunning = true;
    m_thread = thread([](Stepper* pStepper){
        while(pStepper->isKeepRunning()){
            auto wakeTime = chrono::high_resolution_clock::now() + chrono::nanoseconds(MOTOR_STEP_INTERVAL_US);

            int move = pStepper->getTargetPosition() - pStepper->getPosition();

            if(move > 0){
                 pStepper->step(1);
            }else if(move < 0){
                 pStepper->step(-1);
            }


            this_thread::sleep_until(wakeTime);
        }
    }, this);
    m_thread.detach();

}
void Stepper::stop(){
    m_fKeepRunning = false;
    // m_thread.join();
}