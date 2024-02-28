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
    m_stepper.startSweeping(0, MOTOR_SWEEP_ANGLE, MOTOR_STEP_INTERVAL_US);
    thread T1([](Polar *pPolar){
        
    },this);
    T1.detach();
}
void Polar::stop(){
    m_stepper.stopSweeping(0);
}
void Polar::step(int dir){
    m_stepper.step(dir);
}
int Polar::getPosition(){
    return m_stepper.getPosition();
}

void Polar::setBrightness(int val){
    m_ledstring.channel[0].brightness=val;
}