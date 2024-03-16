// g++ testimu.cpp ../wit_c_sdk.c ../imu.cpp -o ../../build/debug/testimu -pthread

#include <iostream>
#include "../imu.hpp"
using namespace std;

int main(int argc, char* argv[]){
    cout << "IMU test" <<endl;
    Imu *pImu = new Imu();

    pImu->start();

    while(1){
        int heading = pImu->getHeading();
        cout << heading << ": " << pImu->getHeadingChange(heading, HEADING_AVERAGE_WINDOW_STEPS) << endl;
        sleep(1);
    }
}
