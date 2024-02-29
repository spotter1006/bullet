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
    // Initialize  frames for animation
    for(int i = 0; i < ANIMATION_FRAMES; i++)
    {
        m_frames.emplace_back(Frame(MOTOR_SWEEP_ANGLE));
    }
    
    // *** Test: single green stationary bar in the middle of a sweep from 0 to max motor sweep angle
    Bar solidGreenBar(10, GREEN); 
    for( int i = 0; i < ANIMATION_FRAMES; i++){
        m_frames[i].setBar(MOTOR_SWEEP_ANGLE/2, solidGreenBar);
    }

    m_stepper.startSweeping(0, MOTOR_SWEEP_ANGLE, MOTOR_STEP_INTERVAL_US);
    // ***

    // Thread to control the LEDs
    thread T1([](Polar *pPolar){
        int frameIndex = 0;
        Frame frame = pPolar->getFrame(frameIndex);
        while(1){
            int step =pPolar->waitForNextStep();
            Bar bar = frame.getBar(step);
            bar.render(pPolar->getLedString());

            if(step == pPolar->getLeftSweepLimit()) frameIndex++;
            if(frameIndex > pPolar->getFrameCount()) frameIndex = 0;
         
            usleep(2);
            if(!pPolar->isKeepRunning()){
                pPolar->setKeepRunning(true);
                break;
            }
        }
    },this);
    T1.detach();
}
void Polar::stop(){
    m_fKeepRunning = false;
    m_stepper.stopSweeping(0);      // Stop at angle 0
    while(!m_fKeepRunning){
        usleep(2);
    }
    
}
void Polar::step(int dir){
    m_stepper.step(dir);
}
int Polar::getPosition(){
    return m_stepper.getPosition();
}
int Polar::getDirection(){
    return m_stepper.getDirection();
}
int Polar::getRightSweepLimit(){
    return m_stepper.getRightSweepLimit();
}
int Polar::getLeftSweepLimit(){
    return m_stepper.getLeftSweepLimit();
}

void Polar::setBrightness(int val){
    m_ledstring.channel[0].brightness=val;
}
int Polar:: waitForNextStep(){
    int initialStep = m_stepper.getPosition();
    int nextStep = initialStep;
    while(nextStep = m_stepper.getPosition() == initialStep){
        usleep(2);
    }
    return nextStep;
}