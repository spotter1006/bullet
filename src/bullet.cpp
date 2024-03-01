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

Bullet::~Bullet(){

}

int Bullet::start(int argc, char *argv[]){
    cout << "Bullet bubble running" << endl;

    // *** TEST code
   
    Ray greenRay(LED_STRING_PIXELS, 0,  1);
    auto init = vector<ws2811_led_t>(10, GREEN);
    greenRay.setPattern(init);
    m_graphicEngine.addElement(greenRay);

    m_polar.start(0, MOTOR_SWEEP_STEPS - 1, MOTOR_STEP_INTERVAL_US);

    // ****

     // Main loop
    string line;
    cout << "Bullet - 'h' for a list of commands" << endl; 
    while(!m_fStop){   
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

    m_polar.stop();
    return 0;
}
void Bullet::stop(int sigNum){
    if(sigNum == SIGINT) m_fStop = true;
}
