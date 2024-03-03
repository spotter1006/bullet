#include "bullet.hpp"
#include <iostream>
#include <signal.h>
#include "polar.hpp"
#include "defines.hpp"
#include "graphicEngine.hpp"

using namespace std;

int Bullet::start(int argc, char *argv[]){
    // Frame buffer shared between graphics engine and polar display
    Frame *pFrame = new Frame(MOTOR_SWEEP_STEPS, LED_STRING_PIXELS);
    m_graphicEngine.setFrame(pFrame);
    m_polar.setFrame(pFrame);
    m_polar.start(0, MOTOR_SWEEP_STEPS - 1, MOTOR_STEP_INTERVAL_US);
    m_graphicEngine.start();
 
    terminal();
    m_graphicEngine.stop();
    m_polar.stop();
    return 0;
}

void Bullet:: terminal(){
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

}
void Bullet::stop(int sigNum){
    if(sigNum == SIGINT) m_fStop = true;
}