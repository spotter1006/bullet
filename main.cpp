#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <thread>

using namespace std;

atomic_flag flagKeepRunning;

void sigHandler(int signum){
    flagKeepRunning.clear();
}

int main(){
    cout << "Bullet bubble running";
    while(flagKeepRunning.test_and_set()){   // Exit on SIGINT
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}