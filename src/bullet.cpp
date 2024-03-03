#include "bullet.hpp"
#include <iostream>
#include <signal.h>
#include "polar.hpp"
#include "defines.hpp"
#include "graphicEngine.hpp"

using namespace std;

Bullet:: Bullet(int left, int right, int pixels, Frame *pFrame):
    m_polar(left, right, pixels, pFrame),
    m_graphicEngine(pFrame, m_polar){m_fStop=false;}

int Bullet::start(int argc, char *argv[]){
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
