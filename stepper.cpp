#include "stepper.hpp"
#include "defines.hpp"
#include <gpiod.hpp>

Stepper::Stepper(){
    m_nPosition = 0;
    m_nMode = 0;
    
    m_lineEnable = chip.getLine(DRV8825_EN_GPIO);   
    m_lineM0 = chip.getLine(DRV8825_M0_GPIO);
    m_lineM1 = chip.getLine(DRV8825_M1_GPIO);
    m_lineM2 = chip.getLine(DRV8825_M2_GPIO);
    m_lineReset = chip.getLine(DRV8825_RESET_GPIO);
    m_lineSleep = chip.getLine(DRV8825_SLEEP_GPIO);
    m_lineStep = chip.getLine(DRV8825_STEP_GPIO);
    m_lineDir = chip.getLine(DRV8825_DIR_GPIO);

    m_lineEnable.request({"liftometer",gpiod::line_request::OUTPUT, 0},0);
    m_lineM0.request({"liftometer",gpiod::m_lineM0::OUTPUT, 0},0);
    m_lineM1.request({"liftometer",gpiod::m_lineM1::OUTPUT, 0},0);
    m_lineM2.request({"liftometer",gpiod::line_request::OUTPUT, 0},0);
    m_lineReset.request({"liftometer",gpiod::line_request::OUTPUT, 0},0);
    m_lineSleep.request({"liftometer",gpiod::line_request::OUTPUT, 0},0);
    m_lineStep.request({"liftometer",gpiod::line_request::OUTPUT, 0},0);
    m_lineDir.request({"liftometer",gpiod::line_request::OUTPUT, 0},0);


}

Stepper::~Stepper(){
    
}
void Stepper::setMode(int mode){
    
}

int Stepper::step(){

}