// g++ testimu.cpp ../wit_c_sdk.c ../imu.cpp -o ../../build/debug/testimu -pthread

#include <iostream>
#include <iomanip>

#include "../imu.hpp"
using namespace std;

int main(int argc, char* argv[]){
    cout << "IMU test" <<endl;
    Imu *pImu = new Imu();
    pImu->start();

    string cmd;
    if(argc > 1) cmd=string(argv[1]);

    if(cmd.compare("rm") == 0){
        char cBuff[1];
        s_cDataUpdate = 0;
        pImu->witWriteReg(KEY, KEY_UNLOCK);
        usleep(200000);
        pImu->witReadReg(HXOFFSET, 1);
        usleep(200000);
        pImu->witReadReg(HYOFFSET, 1);
        usleep(200000);
        pImu->witReadReg(HZOFFSET, 1);
 
        sleep(2);
        vector<int> magOffsets;
        pImu->getMagOffsets(magOffsets);
        cout << "Magnetic bias:" << magOffsets[0] << ", " << magOffsets[1] << ", " << magOffsets[2]  << endl;

    }else if(cmd.compare("cm") == 0){
        cout << "starting magnetic calibration. Hit <enter> to end..." << endl;
        pImu->witStartMagCali();
        getline(cin, cmd);
        pImu->witStopMagCali();
        pImu->witSaveParameter();
        cout << "magnetic calibration ended and parameters saved" << endl;
    }else if(cmd.compare("ca") == 0){
        cout << "starting accelerometer calibration. Hit <enter> to end..." << endl;
        pImu->witStartAccCali();
        getline(cin, cmd);
        pImu->witStopAccCali();
        pImu->witSaveParameter();
        cout << "acceleromter calibration ended and parameters saved" << endl;
    }else if (cmd[0] == 'm'){
        if(cmd.size() != 2){
            cout << "format: m<0,1> where 1 is axis6 mode" << endl;
        }else{
            int mode = stoi(cmd.substr(1));
            pImu->witWriteReg(AXIS6, mode);
            pImu->witSaveParameter();
            cout << "parameters saved" << endl;
        }
    }else{
        cout << "reading data ..." << endl;
        while(1){
            sleep(.75);
            cout << "Accel:\t" 	<< setw(10) << sReg[AX] << setw(10)  << sReg[AY] << setw(10)  << sReg[AZ] << "\n" <<
                "Gyro:\t" 	<< setw(10) << sReg[GX] << setw(10)  << sReg[GY] << setw(10)  << sReg[GZ] << "\n" <<
                "Mag:\t" 	<< setw(10) << sReg[HX] << setw(10) << sReg[HY] << setw(10)  << sReg[HZ] <<  "\n" <<
                "RPY:\t" << setw(10) << sReg[Roll]  << setw(10)  << sReg[Pitch]   << setw(10)  << sReg[Yaw]  << endl;
            cout << "\x1b[A\x1b[A\x1b[A\x1b[A"; //Go back up 4 lines
        }
    }
}
/*
// Scaling example from sample code:
fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
*/
