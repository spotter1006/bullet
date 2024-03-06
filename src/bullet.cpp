#include "bullet.hpp"
#include <iostream>
// #include <signal.h>
#include "polar.hpp"
#include "defines.hpp"
#include "graphicEngine.hpp"

using namespace std;

Bullet:: Bullet(int left, int right, int pixels, Frame *pFrame):
    m_polar(left, right, pixels, pFrame),
    m_graphicEngine(pFrame, m_polar){m_fStop=false;}

int Bullet::start(int argc, char *argv[]){
    // Test
    vector<ws2811_led_t> greenBar(10, GREEN);
    m_graphicEngine.setCurvePattern(5, greenBar);
    // vector<ws2811_led_t> greenBar{BLACK,BLACK,GREEN10,GREEN80,GREEN,GREEN80,GREEN10,BLACK,BLACK};
    // m_graphicEngine.setRadialPattern(5, greenBar);
    // vector<ws2811_led_t> arc(0); 
    // for(int i = 0; i < MOTOR_SWEEP_STEPS; i++){
    // ws2811_led_t color = BLACK;
    // switch(i){
    //     case MOTOR_SWEEP_STEPS / 4:
    //     case MOTOR_SWEEP_STEPS * 3 / 4:
    //         color = WHITE;
    //         break;
    //     case MOTOR_SWEEP_STEPS/2:
    //         color = GREEN;
    //         break;
    //     default:
    //         color = BLACK;
    //         break;
    //   }
    //   arc.push_back(color);
    // }
    // m_graphicEngine.setAxialPattern(MOTOR_SWEEP_STEPS/2, arc);
    // end test

    m_polar.start();
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
