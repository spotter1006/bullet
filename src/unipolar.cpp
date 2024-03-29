#include "unipolar.hpp"
#include "defines.hpp"
#include <cmath>

Unipolar::Unipolar(): 
    m_stepTable{0b1001, 0b1100, 0b0110, 0b0011}, 
    m_nStepIndex(0), 
    m_nPosition(0),
    m_nTargetPosition(0),
    m_fKeepRunning(true)
    {
        using namespace gpiod;
    
        m_line1 =     chip("gpiochip0").get_line(PHASE_1_GPIO);
        m_line1.request({"bullet", line_request::DIRECTION_OUTPUT , 0},0);
        
        m_line2 =     chip("gpiochip0").get_line(PHASE_2_GPIO);
        m_line2.request({"bullet", line_request::DIRECTION_OUTPUT , 0},0);
        
        m_line3 =     chip("gpiochip0").get_line(PHASE_3_GPIO);
        m_line3.request({"bullet", line_request::DIRECTION_OUTPUT , 0},0);
        
        m_line4 =     chip("gpiochip0").get_line(PHASE_4_GPIO);
        m_line4.request({"bullet", line_request::DIRECTION_OUTPUT , 0},0);

        m_thread = thread([](Unipolar* pUnipolar){
            while(pUnipolar->isKeepRunning()){                       
            
                auto wakeTime = chrono::high_resolution_clock::now() + chrono::milliseconds(UNIPOLAR_STEP_INTERVAL_MS); 
                pUnipolar->step();
                this_thread::sleep_until(wakeTime);
            }

        }, this);
        m_thread.detach();
}
Unipolar::~Unipolar(){
    m_fKeepRunning = false;
    m_thread.join();
}

void Unipolar::step(){
    m_mutex.lock();
    if(m_nPosition == m_nTargetPosition){   // Disable all phases when holding position
        m_line1.set_value(0);
        m_line2.set_value(0);
        m_line3.set_value(0);
        m_line4.set_value(0);
    }else{
        int dir = (m_nTargetPosition > m_nPosition)? 1 : -1;

        if(dir == 1 && m_nStepIndex == 3){
            m_nStepIndex = 0;
            m_nPosition++;
        }else if(dir == -1 && m_nStepIndex == 0){
            m_nStepIndex = 3;
            m_nPosition--;
        }else{ 
            m_nStepIndex += dir;
        }

        m_line1.set_value(m_stepTable[m_nStepIndex] && 0b1000);
        m_line2.set_value(m_stepTable[m_nStepIndex] && 0b1000);
        m_line3.set_value(m_stepTable[m_nStepIndex] && 0b1000);
        m_line4.set_value(m_stepTable[m_nStepIndex] && 0b1000);
    }
    m_mutex.unlock();
}

void Unipolar::getPosition(int &position){
    m_mutex.lock();
    position = m_nPosition;
    m_mutex.unlock();
}
void Unipolar::setTargetPosition(int position){
    m_mutex.lock();
    m_nPosition = position;
    m_mutex.unlock();
}
void Unipolar::setTargetPosition(float degrees){
    setTargetPosition(round(degrees * DEGREES_PER_UNIPOLAR_STEP));
}