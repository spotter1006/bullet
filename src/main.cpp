#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <thread>
#include "stepper.hpp"
#include "ws2811.h"

using namespace std;

atomic_flag flagKeepRunning;

void sigHandler(int signum){
    flagKeepRunning.clear();
}

int main(){
    flagKeepRunning.test_and_set();
    cout << "Bullet bubble running" << endl;
    Stepper *pStepper = new Stepper();
    while(flagKeepRunning.test_and_set()){   // Exit on SIGINT
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}