#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <thread>
#include "stepper.hpp"
#include "polar.hpp"

using namespace std;

atomic_flag flagKeepRunning;

void sigHandler(int signum){
    flagKeepRunning.clear();
}

int main(){
    flagKeepRunning.test_and_set();
    cout << "Bullet bubble running" << endl;

    Polar *pPolar = new Polar();
    pPolar->start();
   
    while(flagKeepRunning.test_and_set()){   // Exit on SIGINT
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    pPolar->stop();

    delete pPolar;
}