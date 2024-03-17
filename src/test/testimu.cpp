// g++ testimu.cpp ../wit_c_sdk.c ../imu.cpp -o ../../build/debug/testimu -pthread

#include <iostream>
#include "../imu.hpp"
using namespace std;

int main(int argc, char* argv[]){
    cout << "IMU test" <<endl;
    Imu *pImu = new Imu();

    string cmd;
    if(argc > 1) cmd=string(argv[1]);

    if(cmd.compare("cm") == 0){
        cout << "starting magnetic calibration. Hit <enter> to end..." << endl;
        pImu->witStartMagCali();
        getline(cin, cmd);
        pImu->witStopMagCali();
    }else if(cmd.compare("ca") == 0){
        cout << "starting accelerometer calibration. Hit <enter> to end..." << endl;
        pImu->witStartAccCali();
        getline(cin, cmd);
        pImu->witStopAccCali();
    } 
    sleep(2);
    pImu->start();

    while(1){
        sleep(.5);
        pImu->displayData();
        //Go back up 4 lines
        cout << "\x1b[A";
        cout << "\x1b[A";
        cout << "\x1b[A";
        cout << "\x1b[A";

    }
}
