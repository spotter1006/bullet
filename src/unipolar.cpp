#include "unipolar.hpp"
#include "defines.hpp"
#include <cmath>
#include <unistd.h>

Unipolar::Unipolar(): 
    m_stepTable{
        {0, 1, 1, 0},         
        {0, 0, 1, 1},
        {1, 0, 0, 1},
        {1, 1, 0, 0}
    },




    m_nStepIndex(0), 
    m_nPosition(0),
    m_nTargetPosition(0),
    m_fKeepRunning(true)
    {
        using namespace gpiod;

        m_nStepInterval = UNIPOLAR_MIN_STEP_INTERVAL_US;
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
                // auto wakeTime = chrono::high_resolution_clock::now() + chrono::milliseconds(UNIPOLAR_MIN_STEP_INTERVAL_MS); 
                auto wakeTime = chrono::high_resolution_clock::now() + chrono::microseconds(UNIPOLAR_MIN_STEP_INTERVAL_US);
                pUnipolar->step();
                this_thread::sleep_until(wakeTime);
            }

        }, this);
        m_thread.detach();
}
Unipolar::~Unipolar(){
    m_fKeepRunning = false;
    // m_thread.join();
}
void Unipolar::step(){
    m_line1.set_value(m_stepTable[m_nStepIndex][0]);
    m_line2.set_value(m_stepTable[m_nStepIndex][1]);
    m_line3.set_value(m_stepTable[m_nStepIndex][2]);
    m_line4.set_value(m_stepTable[m_nStepIndex][3]);
    m_nStepIndex++;
    if (m_nStepIndex > 3) m_nStepIndex = 0;

}
// void Unipolar::step(){
//     int position;
//     getPosition(position);
//     int move = m_nTargetPosition - m_nPosition;
//     if(move > 0){
//         m_nStepIndex++;    
//         if(m_nStepIndex > 3) m_nStepIndex = 0;               
//         setOutputs();
//         position++; setPosition(position);
//     }else if(move < 0){
//         m_nStepIndex--;
//         if(m_nStepIndex < 0) m_nStepIndex = 3;
//         setOutputs();
//         position--; setPosition(position);
//     }else{                      // Target position reached
//         m_line1.set_value(0);
//         m_line2.set_value(0);
//         m_line3.set_value(0);
//         m_line4.set_value(0);
//         // // m_nStepInterval = UNIPOLAR_MAX_STEP_INTERVAL_MS;
//     }

//         // // Indexer loigic
//         // if(m_nStepInterval > UNIPOLAR_MIN_STEP_INTERVAL_MS && m_nStepInterval < UNIPOLAR_MAX_STEP_INTERVAL_MS){
//         //     float v = 1000.0f / m_nStepInterval;
//         //     float stoppingDistance = (v * v) / (2.0f * UNIPOLAR_ACCEL); // Stopping distance = v^2/2a
//         //     if(move > stoppingDistance )              
//         //         m_nStepInterval -= UNIPOLAR_ACCEL;      // Accelerate
//         //     else
//         //         m_nStepInterval += UNIPOLAR_ACCEL;      // Deccelerate
//         // }

        // }
        // }

// }
// }

// }
void Unipolar::setOutputs(){
    m_line1.set_value(m_stepTable[m_nStepIndex][0]);
    m_line2.set_value(m_stepTable[m_nStepIndex][1]);
    m_line3.set_value(m_stepTable[m_nStepIndex][2]);
    m_line4.set_value(m_stepTable[m_nStepIndex][3]);
}

// Thread safe accessors
void Unipolar::getPosition(int &position){
    m_mutex.lock();
    position = m_nPosition;
    m_mutex.unlock();
}
void Unipolar::setPosition(int &position){
    m_mutex.lock();
    m_nPosition = position;
    m_mutex.unlock();
}
void Unipolar::setTargetPosition(int position){
    m_mutex.lock();
    m_nTargetPosition = position;
    m_mutex.unlock();
}
void Unipolar::setTargetPosition(float degrees){
    setTargetPosition((int)round(degrees * UNIPOLAR_STEPS_PER_DEGREE));
}
void Unipolar::reset(){
    m_mutex.lock();
    m_nPosition = 0;
    m_nStepIndex = 0;
    m_nTargetPosition=0;
    m_mutex.unlock();
}
void Unipolar::home(){
    setTargetPosition(UNIPOLAR_TRAVEL * 1.5f);
    sleep(1);
    reset();
    setTargetPosition(-UNIPOLAR_TRAVEL / 2.0f);
    sleep(1);
    reset();
}