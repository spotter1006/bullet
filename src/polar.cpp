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
    Frame frame = Frame(MOTOR_SWEEP_STEPS);
    m_frames = vector<Frame>(ANIMATION_FRAMES, frame);

// *** Test: single green stationary bar in the middle of a sweep from 0 to max motor sweep angle
    Bar solidGreenBar(10, GREEN);
    for( int i = 0; i < ANIMATION_FRAMES; i++){
        m_frames[i].setBar(MOTOR_SWEEP_STEPS/2, solidGreenBar);
    } 
}
Polar::~Polar(){
    ws2811_fini(&m_ledstring);
}
atomic_flag fWaitForTick;   
void Polar::start(int left, int right, int stepIntervalUs){
    m_nLeftSweepLimit = left;
    m_nRightSweepLimit = right;

    signal(SIGALRM, [](int signo){fWaitForTick.clear();});   
    itimerval timer;
    timer.it_interval.tv_usec = stepIntervalUs;      
    timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = stepIntervalUs;
    timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    
    m_fKeepSweeping = true;   
    m_threads.emplace_back(thread([](Polar *pPolar){        //Motion thread
        Frame frame = pPolar->getFrame(0);
        pPolar->setKeepSweeping(true);
        int dir = 1;
        while(pPolar->isKeepRunning()){
            while(fWaitForTick.test_and_set()){
                usleep(2);
            };
            int step = pPolar->step(dir);
            if(step == pPolar->getRightSweepLimit())  dir = -1;
            else if(step == pPolar->getLeftSweepLimit())  dir = 1;
        }
    },this));    
    m_threads.emplace_back(thread([](Polar *pPolar){        // LED thread
        int step = 0;
        int frameIndex = 0;
        
        while(pPolar->isKeepRunning()){
            usleep(2);
            step = pPolar->getStep();
            Frame frame = pPolar->getFrame(frameIndex);
            Bar bar = frame.getBar(step);
            bar.render(pPolar->getLedString());
            if(step == pPolar->getLeftSweepLimit()) frameIndex++;
            if(frameIndex == pPolar->getFrameCount()) frameIndex = 0;
        }
    },this));
}
void Polar::stop(){
    m_fKeepSweeping = false;
    for(auto& th : m_threads)
        th.join();
    m_threads.clear();
}