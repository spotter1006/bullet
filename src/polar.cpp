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

    m_pLedstring = new ws2811_t;        // Structure that holds the context for the ws2811 library
    m_pLedstring->freq = LED_STRING_FREQUENCY;
    m_pLedstring->dmanum =10;
    m_pLedstring->channel[0].gpionum = WSS2812_DATA_GPIO;
    m_pLedstring->channel[0].invert = 0;
    m_pLedstring->channel[0].count = LED_STRING_PIXELS;
    m_pLedstring->channel[0].strip_type = WS2811_STRIP_GRB;
    m_pLedstring->channel[0].brightness = 255;
    m_pLedstring->channel[1].gpionum = 0;
    m_pLedstring->channel[1].invert = 0;
    m_pLedstring->channel[1].count = 0;
    m_pLedstring->channel[1].strip_type = 0;
    m_pLedstring->channel[1].brightness = 0;
    ws2811_init(m_pLedstring);
    m_pStepper = new Stepper();
}
Polar::~Polar(){
    delete m_pStepper;
    delete m_pLedstring;
}
void Polar::start(){
    thread t1(sweeper, this);
    t1.detach();
}
void Polar::stop(){
    fKeepRunning.clear();
}
void Polar::step(int dir){
    m_pStepper->step(dir);
}
int Polar::getPosition(){
    return m_pStepper->getPosition();
}
void Polar::render(){
    ws2811_render(m_pLedstring);
}

void Polar::blankString(){
    for(int i = 0; i < LED_STRING_PIXELS; i++){
        m_pLedstring->channel[0].leds[i] = 0;
    }
    render();
}
void Polar::setPixel(int pixel, int value){
    m_pLedstring->channel[0].leds[pixel] = value;
}

void Polar::setBrightness(int val){
    m_pLedstring->channel[0].brightness=val;
}

void Polar::sweeper(Polar* pPolar){

    signal(SIGALRM, [](int signo){fWaitForTick.clear();});       
    itimerval timer;
 
    timer.it_interval.tv_usec = 180;      
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = 180;
    timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);


    fKeepRunning.test_and_set();
    pPolar->blankString();
    pPolar->setPixel(9, 0x00008000);
    pPolar->render();
    int dir = 1;
    while(fKeepRunning.test_and_set()){

        pPolar->step(dir);
        int position = pPolar->getPosition();
        if(position == 400)  dir = -1;
        else if(position == 0)  dir = 1;
 
        
        while(fWaitForTick.test_and_set())
            usleep(2);
    }
}

