#include "polar.hpp"
#include "defines.hpp"
#include <thread>
#include <iostream>
#include <signal.h>
#include <thread>
#include <sys/time.h>
#include <unistd.h>
#include <atomic>
#include <string.h>
using namespace std;

atomic_flag fWaitForTick;
atomic_flag fKeepRunning;

Polar::Polar(){
 
    m_ledstring.freq = LED_STRING_FREQUENCY;
    m_ledstring.dmanum =10;
    m_ledstring.channel[0].gpionum = WSS2812_DATA_GPIO;
    m_ledstring.channel[0].invert = 0;
    m_ledstring.channel[0].count = LED_STRING_PIXELS;
    m_ledstring.channel[0].strip_type = WS2811_STRIP_GRB;
    m_ledstring.channel[0].brightness = 255;
    m_ledstring.channel[1].gpionum = 0;
    m_ledstring.channel[1].invert = 0;
    m_ledstring.channel[1].count = 0;
    m_ledstring.channel[1].strip_type = 0;
    m_ledstring.channel[1].brightness = 0;
    ws2811_init(&m_ledstring);

}
Polar::~Polar(){
    ws2811_fini(&m_ledstring);
}
void Polar::start(){
    thread t1(sweeper, this);
    t1.detach();
}
void Polar::stop(){
    fKeepRunning.clear();
}
void Polar::step(int dir){
    m_stepper.step(dir);
}
int Polar::getPosition(){
    return m_stepper.getPosition();
}
void Polar::setMotorStepInterval(int microseconds){
    signal(SIGALRM, [](int signo){fWaitForTick.clear();});       
    itimerval timer;
    timer.it_interval.tv_usec = microseconds;      
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = microseconds;
    timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
}


void Polar::setBrightness(int val){
    m_ledstring.channel[0].brightness=val;
}

void Polar::sweeper(Polar* pPolar){

    pPolar->setMotorStepInterval(180);
    
    int dir = 1;
    fKeepRunning.test_and_set();
    while(fKeepRunning.test_and_set()){

        pPolar->step(dir);
        int position = pPolar->getPosition();
        if(position == 400)  dir = -1;
        else if(position == 0)  dir = 1;
 
        
        while(fWaitForTick.test_and_set())
            usleep(2);
    }
}

