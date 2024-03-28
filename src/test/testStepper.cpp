#include "../stepper.hpp"
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]){
    Stepper* pStepper = new Stepper(0);
    pStepper->start();

    cout << "Set taget position to 1000"<< endl;
    pStepper->setTargetPosition(100);
    sleep(.5);
    cout << "Set taget position to 0"<< endl;
    pStepper->setTargetPosition(0);
   
}
