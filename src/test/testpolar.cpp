#include "../polar.hpp"
#include <iostream>
#include<sstream>
#include <iomanip>



int main(int argc, char* argv[] ){
    Polar* pPolar = new Polar();
    pPolar->start();

    cout << "Running. <ctrl-c> to stop ..." << endl;
    while(1){
        sleep(1);
        cout << "average heading:\t" <<setw(10) << pPolar->getAverageHeading() << "\n";
        cout << "accel.y:\t" << setw(10) << pPolar->getLinearAcceleration().axis.y << endl;
        cout << "\x1b[A\x1b[A"; //Go back up 2
    }

}

