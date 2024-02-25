#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <thread>
#include "stepper.hpp"
#include <ws2811/ws2811.h>

using namespace std;

atomic_flag flagKeepRunning;

void sigHandler(int signum){
    flagKeepRunning.clear();
}

ws2811_led_t displayData[10] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x0000ff00,
    0x00000000
};
int main(){
    flagKeepRunning.test_and_set();
    cout << "Bullet bubble running" << endl;
    Stepper *pStepper = new Stepper();

    ws2811_t ledstring=
    {
        .freq = 800000,
        .dmanum = 10,
        .channel =
        {
            [0] =
            {
                .gpionum = 18,
                .invert = 0,
                .count = 10,
                .strip_type = WS2811_STRIP_GBR,
                .brightness = 255,
            },
            [1] =
            {
                .gpionum = 0,
                .invert = 0,
                .count = 0,
                .brightness = 0,
            },
        },
    };
    ws2811_init(&ledstring);
    ledstring.channel[0].leds = displayData;     
    
    ws2811_render(&ledstring);
    while(flagKeepRunning.test_and_set()){   // Exit on SIGINT
        this_thread::sleep_for(chrono::milliseconds(500));

        
    }

    ws2811_fini(&ledstring);
}