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


const ws2811_led_t gradient[] = {                       // https://colordesigner.io/gradient-generator
    0x00fe0101, 0x00fe2302, 0x00fe4503, 0x00fe6704,     // green to red
    0x00fe8805, 0x00feaa06, 0x00fecb07, 0x00feec08,
    0x00f1ff08, 0x00d1ff09, 0x00b1ff0a, 0x0091ff0b,
    0x0071ff0c, 0x0052ff0d, 0x0033ff0e, 0x0014ff0f
};



Polar::Polar(int left, int right, int radius):
    m_nLeftSweepLimit(left),
    m_nRightSweepLimit(right),
    m_nInterval(100),
    m_nAngle(0),
    m_chaser(radius),
    m_intensities({0, 0, 3, 7, 27, 35, 58, 89, 129, 180}),      // Gamma corrected ramp
    m_colors(radius, BLUE)
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

            int move = pPolar->getAngle() - pPolar->getMotorPosition();
            if(move > 0){
                if(pPolar->getMotorPosition() < pPolar->getRightSweepLimit()) pPolar->stepMotor(1);
            }else if(move < 0){
                if(pPolar->getMotorPosition() > pPolar->getLeftSweepLimit()) pPolar->stepMotor(-1);
            }

        }
    },this));
}
void Polar::stop(){
    m_fKeepSweeping = false;
    for(auto& th : m_threads)
        th.join();
    m_threads.clear();
}

ws2811_led_t Polar::redToGreen(int val){
    return gradient[(val + 128) / 16];
   
}

void Polar::setHue(int hue){
    m_colors = vector<ws2811_led_t>(m_colors.size(), redToGreen(hue));  // Monotone 
    m_chaser.setPattern(m_intensities, m_colors);
}