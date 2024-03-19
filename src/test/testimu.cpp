// g++ testimu.cpp ../wit_c_sdk.c ../imu.cpp -o ../../build/debug/testimu -pthread

#include <iostream>
#include <iomanip>

#include "../imu.hpp"
using namespace std;

int main(int argc, char* argv[]){
    cout << "IMU test" <<endl;
    Imu *pImu = new Imu();
    pImu->start();
    sleep(2);

    string cmd;
    if(argc > 1) cmd=string(argv[1]);

    if(cmd.compare("rs") == 0){
        char cBuff[1];
        s_cDataUpdate = 0;
        pImu->readSettings();  
        sleep(2);   
        vector<uint16_t> settings;
        pImu->getSettings(settings);
        cout << "Settings:" << endl;

        uint baud = 0;
        switch(settings[2] & 0x07){
            case 0x01: baud =4800; break;
            case 0x02: baud =9600; break;
            case 0x03: baud =19200; break;
            case 0x04: baud =38400; break;
            case 0x05: baud =57600; break;
            case 0x06: baud =115200; break;
            case 0x07: baud =230400; break;
            case 0x08: baud =460800; break;
            case 0x09: baud =921600; break;
        }
        string outputRate = "";
        switch(settings[1] & 0x07){
            case 0x01: outputRate = "0.2Hz"; break;
            case 0x02: outputRate = "0.5Hz"; break;
            case 0x03: outputRate = "1Hz"; break;
            case 0x04: outputRate = "2Hz"; break;
            case 0x05: outputRate = "5Hz"; break;
            case 0x06: outputRate = "10Hz"; break;
            case 0x07: outputRate = "20Hz"; break;
            case 0x08: outputRate = "50Hz"; break;
            case 0x09: outputRate = "100Hz"; break;
            case 0x0B: outputRate = "200Hz"; break;
            case 0x0C: outputRate = "single return"; break;
            case 0x0D: outputRate = "no return"; break;
        }
        string features[11] = {"TIME ","ACC ","GYRO ","ANGLE ","MAG ","PORT ","PRESS ","GPS ","VELOCITY ","QUATER ","GSA "};
        string outputs = "";
        uint bits = settings[0] & 0x17;
        uint mask = 0x0001;
        for(int i = 0; i < 11; i++){
            if(bits & mask) outputs += features[i];
            mask = mask << 1;
        }

        string axis6 = (settings[3] == 0x01)? "on":"off";

        cout << "Output content: " << outputs << endl;
        cout << dec << "Output rate: " << outputRate << endl;
        cout << dec << "Baud rate: " << baud << endl;
        cout << "Axis6 algorithm: " << axis6 << endl;
        
    }else if(cmd.compare("rb") == 0){
        char cBuff[1];
        s_cDataUpdate = 0;
        pImu->readBiases();     
        vector<uint16_t> offsets;
        pImu->getBiasTable(offsets);
        cout << "Bias Table:" << endl;
        string vals[3] = {"Accel\t","Gyro\t","Mag\t"};
        for(int i = 0; i <3; i++){
            cout << vals[i];
            for(int j = 0; j < 3; j++){
                cout << setw(6) << offsets[i + j] << " ";
            }
            cout << endl;
        }

    }else if(cmd.compare("cm") == 0){
        cout << "starting magnetic calibration. Hit <enter> to end..." << endl;
        pImu->witStartMagCali();
        getline(cin, cmd);
        pImu->witStopMagCali();
        usleep(200000);
        pImu->witSaveParameter();
        usleep(200000);
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
