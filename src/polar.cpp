#include "polar.hpp"
#include "defines.hpp"
#include <thread>
#include <signal.h>
#include <thread>
#include <unistd.h>
#include <atomic>
#include <string.h>
using namespace std;

atomic_flag fWaitForTick;   

Polar::Polar(int left, int right, int radius):
    m_nLeftSweepLimit(left),
    m_nRightSweepLimit(right),
    m_nInterval(100),
    m_nAngle(0),
    m_chaser(radius),
    m_intensities({0, 0, 3, 7, 27, 35, 58, 89, 129, 180}),      // Gamma corrected ramp
    m_colors(radius, BLUE), 
    m_imu()
    {
        signal(SIGALRM, [](int signo){fWaitForTick.clear();});   
        itimerval timer;
        timer.it_interval.tv_usec = timer.it_value.tv_usec = MOTOR_STEP_INTERVAL_US;      
        timer.it_interval.tv_sec = timer.it_value.tv_sec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);
        m_fKeepSweeping = true;
        m_chaser.setPattern(m_intensities, m_colors);
}

void Polar::start(){
    
    m_threads.emplace_back(thread([](Polar *pPolar){   
        unsigned int timeTick = 0;
        while(pPolar->isKeepSweeping()){                       
            while(fWaitForTick.test_and_set()) usleep(2);   // Wait for precise time interval
            timeTick++;       
           
            if(pPolar->getInterval() != 0 && timeTick % abs(pPolar->getInterval()) == 0){         
                pPolar->chaserRotate(pPolar->getInterval() < 0? -1 : 1);    
            } 

            int headingChange = pPolar->getHeadingChange(HEADING_AVERAGE_WINDOW_STEPS);     
            pPolar->setAngle(headingChange);
            
            int move = pPolar->getAngle() - pPolar->getMotorPosition();
            if(move > 0){
                if(pPolar->getMotorPosition() < pPolar->getRightSweepLimit()) pPolar->stepMotor(1);
            }else if(move < 0){
                if(pPolar->getMotorPosition() > pPolar->getLeftSweepLimit()) pPolar->stepMotor(-1);
            }


        }
    },this));
    m_imu.start();
}
void Polar::stop(){
    m_imu.stop();
    m_fKeepSweeping = false;
    for(auto& th : m_threads)
        th.join();
    m_threads.clear();
}

void Polar::setHue(int hue){
    m_colors = vector<ws2811_led_t>(m_colors.size(), redToGreen(hue));  // Monotone 
    m_chaser.setPattern(m_intensities, m_colors);
}

void Polar::home(){

    while(m_stepper.getPosition() > m_nLeftSweepLimit * 2){              // Move way past the limit stalling the motor at the limit
        m_stepper.step(-1);
        usleep(MOTOR_STEP_INTERVAL_US * 16);         // 1/16 speed
    }

    for(int i = 0; i < (m_nRightSweepLimit - m_nLeftSweepLimit)/2; i++){       
        m_stepper.step(1);
        usleep(MOTOR_STEP_INTERVAL_US * 16);             
    }

    for(int i = 0; i< 8; i++){
        m_stepper.step(-1);
        usleep(MOTOR_STEP_INTERVAL_US * 16);  
    }

    setAngle(0);
    m_stepper.zeroPosition();
    m_stepper.resetPulse();         // Resets the indexer to electrical angle 45 degrees
}