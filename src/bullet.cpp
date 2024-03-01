#include "bullet.hpp"
#include <gpiod.hpp>
#include <iostream>
#include <signal.h>
#include <thread>
#include "stepper.hpp"
#include "polar.hpp"
#include "defines.hpp"
#include "graphicEngine.hpp"
#include "ray.hpp"
using namespace std;

atomic_flag flagKeepRunning;
void sigHandler(int signum){
    flagKeepRunning.clear();
}

Bullet::~Bullet(){

}


int Bullet::main(int argc, char *argv[]){
    flagKeepRunning.test_and_set();
    cout << "Bullet bubble running" << endl;

    Polar *pPolar = new Polar();
    m_polar.start(0, MOTOR_SWEEP_STEPS - 1, MOTOR_STEP_INTERVAL_US);
   
    GraphicEngine *pGraphicEngine = new GraphicEngine(1, MOTOR_SWEEP_STEPS, LED_STRING_PIXELS);
    Ray greenRay(LED_STRING_PIXELS, 0,  1);
    

    pGraphicEngine->addElement(greenRay);

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
            cout << "q - quit bullet" << endl;
        }
    }

    pPolar->stop();

    delete pPolar;
    delete pGraphicEngine;
    return 0;
}
