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
        cout << "Heading change: " << setw(10) << pPolar->getHeadingChange() << " Average: " << setw(10) << pPolar->getAverageHeading() << endl;
        cout << "\x1b[A"; //Go back up 1 line
    }

}

