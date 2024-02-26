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
   

     // Main loop
    string line;
    cout << "Bullet - 'h' for a list of commands" << endl; 
    while(flagKeepRunning.test_and_set()){   // Exit on SIGINT
        getline(cin, line);
        if(line.compare("q") == 0){
            cout << "Quit command recieved, exiting..." << endl;
            break;
        }else if(line.compare("h") == 0){
            cout << "Bullet commands:" << endl;
            cout << "h - diplay this help message" << endl;
            cout << "b - set brightness" << endl;
            cout << "q - quit bullet" << endl;
        }else if(line.find("b", 0, 1) == 0){
            string sval = line.substr(1);
            cout << "Setting brightess = " << sval << endl;
            int val = stoi(sval);
            pPolar->setBrightness(val);
        }
    }

    pPolar->stop();

    delete pPolar;
}