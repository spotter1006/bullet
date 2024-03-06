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

ws2811_led_t pixel(uint8_t red, uint8_t green, uint8_t blue, double intensity){
    uint32_t sred = red * intensity;
    uint32_t sgreen = red * intensity;
    uint32_t sblue = red * intensity;
    return sred << 16 | sgreen << 8 | sblue;

}
Polar::Polar(int left, int right, int radius):
    m_nLeftSweepLimit(left),
    m_nRightSweepLimit(right),
    m_chaser(radius){
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
            while(fWaitForTick.test_and_set()) usleep(2);
            
            timeTick++;           
            if(timeTick % 150 == 0){         // test
                pPolar->chaserRotate(1);    
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

