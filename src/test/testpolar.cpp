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
        FusionVector accel=  pPolar->getLinearAcceleration();
        FusionEuler orientation = pPolar->quaternionToEuler(pPolar->getQuaternion());
        cout << "heading:\t" << setw(10) << orientation.angle.yaw << "\n";
        cout << "average:\t" <<setw(10) << pPolar->getAverageHeading() << "\n";
        cout << "change: \t" << setw(10) << pPolar->getHeadingChange() << endl;
        cout << "\x1b[A\x1b[A\x1b[A"; //Go back up 3
    }

}

