#include "polar.hpp"
#include "defines.hpp"
#include <thread>
#include <signal.h>
#include <thread>
#include <sys/time.h>
#include <unistd.h>
#include <atomic>
#include <string.h>
using namespace std;

atomic_flag fWaitForTick;   
Polar::Polar(int left, int right, int radius, Frame* pFrame):
    m_strobe( 1, 1, 1){
    m_nLeftSweepLimit = left;
    m_nRightSweepLimit = right;
    m_nRadius = radius;
    
    m_ledstring.freq = LED_STRING_FREQUENCY;
    m_ledstring.dmanum =10;
    m_ledstring.channel[0].gpionum = WSS2812_DATA_GPIO;
    m_ledstring.channel[0].invert = 0;
    m_ledstring.channel[0].count = radius;
    m_ledstring.channel[0].strip_type = WS2811_STRIP_GRB;
    m_ledstring.channel[0].brightness = 255;
    m_ledstring.channel[1].gpionum = 0;
    m_ledstring.channel[1].invert = 0;
    m_ledstring.channel[1].count = 0;
    m_ledstring.channel[1].strip_type = 0;
    m_ledstring.channel[1].brightness = 0;
    ws2811_init(&m_ledstring);
    m_pFrame = pFrame;    
    m_nFrame = 0;

    signal(SIGALRM, [](int signo){fWaitForTick.clear();});   
    itimerval timer;
    timer.it_interval.tv_usec = timer.it_value.tv_usec =MOTOR_STEP_INTERVAL_US;      
    timer.it_interval.tv_sec = timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    m_fKeepSweeping = true;
}
Polar::~Polar(){
    ws2811_fini(&m_ledstring);
}

void Polar::start(){

    m_threads.emplace_back(thread([](Polar *pPolar){
        pPolar->setKeepSweeping(true);
        int dir = 1;
        int step = pPolar->getStep();
        int leftLimit = pPolar->getLeftSweepLimit();
        int rightLimit = pPolar->getRightSweepLimit() - 1;
        while(pPolar->isKeepSweeping()){
            while(fWaitForTick.test_and_set()) usleep(MOTOR_STEP_INTERVAL_US / 10);

            if(step == leftLimit && dir == -1) pPolar->incrementFrame();
            step = pPolar->step(dir);
            if(step == rightLimit)  dir = -1;
            else if(step == pPolar->getLeftSweepLimit())  dir = 1;
           
        }
    },this));    
    m_threads.emplace_back(thread([](Polar *pPolar){         
        while(pPolar->isKeepSweeping()){
            ws2811_t *ledString = pPolar->getLedString();
            pPolar->copyBarData(ledString->channel[0].leds, pPolar->getStep());
            pPolar->shutter(ledString, pPolar->getFrameNumber(), pPolar->getDirection());
            ws2811_render(ledString);
            usleep(MOTOR_STEP_INTERVAL_US / 2);
        }
    },this));
}
void Polar::stop(){
    m_fKeepSweeping = false;
    for(auto& th : m_threads)
        th.join();
    m_threads.clear();
}