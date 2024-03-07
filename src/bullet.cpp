#include "bullet.hpp"
#include <iostream>
#include "polar.hpp"
#include "defines.hpp"

using namespace std;

int Bullet::start(int argc, char *argv[]){
    m_polar.start();
    terminal();
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
        }else if(line[0] == 'a'){
            int angle = stoi(line.substr(1));
            cout << "Setting angle to " << angle << endl;
            m_polar.setAngle(angle);
        
        }else if(line[0] == 'i'){
            int interval = stoi(line.substr(1));
            cout << "Setting interval to " << interval << endl;
            m_polar.setIterval(interval);
        
        }else if(line.compare("h") == 0){
            cout << "Bullet commands:" << endl;
            cout << "h - diplay this help message" << endl;
            cout << "q - quit bullet" << endl;
            cout << "a<angle> - set the display pointer angle" << endl;
            cout << "i<interval> - set the interval for the chaser" << endl;
        }
    }
}
