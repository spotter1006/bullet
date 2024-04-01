#include "../unipolar.hpp"
#include <iostream>
#include <unistd.h>




int main(int argc, char* argv[]){
    Unipolar* pUnipolar = new Unipolar();
    cout << "moving back and forth. <ctrl-c> to end" << endl;


    while(1){
       pUnipolar->setTargetPosition(-45.0f);     
        sleep(1);
        pUnipolar->setTargetPosition(45.0f);
        sleep(1);

    }
}
