#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <thread>
#include "stepper.hpp"
#include "ledstring.hpp"


using namespace std;

atomic_flag flagKeepRunning;

void sigHandler(int signum){
    flagKeepRunning.clear();
}


int main(){
    flagKeepRunning.test_and_set();
    cout << "Bullet bubble running" << endl;
    Stepper *pStepper = new Stepper();
    Ledstring *pLeds = new Ledstring(10);

    pLeds->clear();
    pLeds->setPixel(9, 0x00808000);
    pLeds->render();

    
    while(flagKeepRunning.test_and_set()){   // Exit on SIGINT
        this_thread::sleep_for(chrono::milliseconds(500));
    }


    delete pStepper;
    delete pLeds;
}