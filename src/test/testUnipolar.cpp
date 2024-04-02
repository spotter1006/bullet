#include "../unipolar.hpp"
#include <iostream>
#include <unistd.h>




int main(int argc, char* argv[]){
    Unipolar* pUnipolar = new Unipolar();
    cout << "moving back and forth. <ctrl-c> to end" << endl;

    pUnipolar->home();

    while(1){
       pUnipolar->setTargetPosition(-15.0f);     
        sleep(1);
        pUnipolar->setTargetPosition(15.0f);
        sleep(1);

    }
}
