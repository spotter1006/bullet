#include "stepper.hpp"
#include "defines.hpp"
#include <gpiod.hpp>
#include <thread>

Stepper::Stepper(){
    m_nPosition = 0;
    
    m_lineEnable =  m_ioChip.getLine(DRV8825_EN_GPIO);   
    m_lineM0 =      m_ioChip.getLine(DRV8825_M0_GPIO);
    m_lineM1 =      m_ioChip.getLine(DRV8825_M1_GPIO);
    m_lineM2 =      m_ioChip.getLine(DRV8825_M2_GPIO);
    m_lineReset =   m_ioChip.getLine(DRV8825_RESET_GPIO);
    m_lineSleep =   m_ioChip.getLine(DRV8825_SLEEP_GPIO);
    m_lineStep =    m_ioChip.getLine(DRV8825_STEP_GPIO);
    m_lineDir =     m_ioChip.getLine(DRV8825_DIR_GPIO);

    m_lineEnable.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineM0.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineM1.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineM2.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineReset.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineSleep.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineStep.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);
    m_lineDir.request({"liftometer",gpiod::line_request::DIRECTION_OUTPUT , 0},0);

    m_lineSleep.set_value(0);
    m_lineEnable.set_value(0);
    m_lineReset.set_value(0);
    m_lineM0.set_value(0);
    m_lineM1.set_value(0);
    m_lineM2.set_value(0);
    
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


int Stepper::step(int dir){
    m_lineDir.set_value(dir);
    m_lineStep.set_value(1);
    this_thread.sleep_for(chrono::milliseconds(1));
    m_lineStep.set_value(1);
    m_nPosition += (dir)? 1: -1;
}