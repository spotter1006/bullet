#include "stepper.hpp"
#include "defines.hpp"
#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

Stepper::Stepper(int microstepMode){
    m_fKeepRunning = true;
    m_nPosition = 0;
    m_nTargetPosition = 0;
    m_nStepIntervalUs = MOTOR_MAX_STEP_INTERVAL_US;
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
        
    setMicrostepping(microstepMode);    // Full step
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

int Stepper::step(){
    if(m_nPosition == m_nTargetPosition){ 
        m_nStepIntervalUs = MOTOR_MAX_STEP_INTERVAL_US;
    }else{
        step((m_nPosition > m_nTargetPosition)? -1 : 1);
    }

    if(m_nStepIntervalUs > MOTOR_MIN_STEP_INTERVAL_US && m_nStepIntervalUs < MOTOR_MAX_STEP_INTERVAL_US)
        m_nStepIntervalUs -= MOTOR_ACCEL;
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
            auto wakeTime = chrono::high_resolution_clock::now() + chrono::microseconds(pStepper->getStepInterval());
            pStepper->step();
            this_thread::sleep_until(wakeTime);
        }
    }, this);
    m_thread.detach();

}
void Stepper::stop(){
    m_fKeepRunning = false;
    // m_thread.join();
}


void Stepper::setMicrostepping(char unsigned val){
    /*
        val MODE2   MODE1   MODE0   STEP MODE
        0    0       0       0       Full step (2-phase excitation) with 71% current
        1    0       0       1       1/2 step (1-2 phase excitation)
        2    0       1       0       1/4 step (W1-2 phase excitation)
        3    0       1       1       8 microsteps/step
        4    1       0       0       16 microsteps/step
        5    1       0       1       32 microsteps/step
        6    1       1       0       32 microsteps/step
        7    1       1       1       32 microsteps/step
    */

    m_lineM0.set_value(val & 0x01);
    m_lineM1.set_value(val & 0x02);
    m_lineM2.set_value(val & 0x04);

}

