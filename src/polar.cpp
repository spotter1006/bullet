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

const uint8_t gamma[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
    115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
    144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
    177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
    215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


Polar::Polar(int left, int right, int radius):
    m_nLeftSweepLimit(left),
    m_nRightSweepLimit(right),
    m_nInterval(100),
    m_nAngle(0),
    m_chaser(radius),
    m_intensities({0, 2, 8, 32, 128, 0, 2, 8, 32, 128}),
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
    int green = val + 128;
    int red = 128 - val;
    return gamma[red]  << 16 | gamma[green] << 8;
}

void Polar::setHue(int hue){
    m_colors = vector<ws2811_led_t>(m_colors.size(), redToGreen(hue));  // Monotone 
    m_chaser.setPattern(m_intensities, m_colors);
}