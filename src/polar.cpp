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
    m_chaser(radius)
    {
        signal(SIGALRM, [](int signo){fWaitForTick.clear();});   
        itimerval timer;
        timer.it_interval.tv_usec = timer.it_value.tv_usec = MOTOR_STEP_INTERVAL_US;      
        timer.it_interval.tv_sec = timer.it_value.tv_sec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);
        m_fKeepSweeping = true;
}

void Polar::start(){
    // *** test ***
    vector<ws2811_led_t> testPattern = {0, 2, 8, 32, 128, 0, 2, 8, 32, 128};
    m_chaser.setPattern(testPattern);
    // *** test ***

    m_threads.emplace_back(thread([](Polar *pPolar){   
        unsigned int timeTick = 0;
        while(pPolar->isKeepSweeping()){                       
            while(fWaitForTick.test_and_set()) usleep(2);   // Wait for precise time interval
            timeTick++;       
           
            if(pPolar->getInterval() != 0 && timeTick % abs(pPolar->getInterval()) == 0){         
                pPolar->chaserRotate(pPolar->getInterval() < 0? -1 : 1);    
            }    

            int move = pPolar->getMotorPosition() - pPolar->getAngle();
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