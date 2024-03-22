// g++ testimu.cpp ../wit_c_sdk.c ../imu.cpp -o ../../build/debug/testimu -pthread

#include <iostream>
#include <iomanip>

#include "../imu.hpp"
using namespace std;

void showSettings(Imu* pImu){
    char cBuff[1];
 
        vector<uint16_t> settings;
        pImu->getSettings(settings);    // Thread safe

        string bauds[10] ={"invalid", "4800","9600","19200","38400","57600","115200","230400","460800","921600"};
        string baud=bauds[(settings[2] & 0x07)];

        string rates[13]={"invalid", "0.2","0.5","1","2","5","10","20","50","100","200","single return","no return"};
        string outputRate = rates[(settings[1] & 0x07)];

        string features[11] = {"TIME ","ACC ","GYRO ","ANGLE ","MAG ","PORT ","PRESS ","GPS ","VELOCITY ","QUATER ","GSA "};
        string outputs = "";
        uint bits = settings[0] & 0x17;
        uint mask = 0x0001;
        for(int i = 0; i < 11; i++){
            if(bits & mask) outputs += features[i];
            mask = mask << 1;
        }

        string axis6 = (settings[3] == 0x01)? "on":"off";

        string bandwidths[7] = {"256Hz","188Hz","98Hz","42Hz","20Hz","10Hz","5Hz"};
        string bandwidth=bandwidths[settings[4] & 0x07];

        cout << "Output content: " << outputs << endl;
        cout << "Output rate: " << outputRate << endl;
        cout << "Baud rate: " << baud << endl;
        cout << "Axis6 algorithm: " << axis6 << endl;
        cout << "bandwidth: " << bandwidth << endl;
        
}
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
        showSettings(pImu);
       
        
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
            pImu->setAxis6(mode);
            pImu->witSaveParameter();
            cout << "axis6 algorithm set to " << mode << endl;
        }
    }else if(cmd[0] == 'b'){
        if(cmd.size() != 2){
            cout << "format: b<1..10> for .2 to 200, 11 single return, 12 no return>" << endl;
        }else{
            int bw = stoi(cmd.substr(1));
            pImu->setBandwidth(bw);
            pImu->witSaveParameter();
            cout << "bandwidth set to " << bw << endl;
        }
    }else if (cmd[0] == 'r'){
        if(cmd.size() != 2){
            cout << "output rate: 1-13 for 0.2Hz..200Hz, single or none" << endl;
        }else{
            int rate = stoi(cmd.substr(1));
            pImu->witSetOutputRate(rate);
            pImu->witSaveParameter();
            cout << "parameters saved" << endl;
        }
    }else{
        while(1){
            sleep(.75);
            FusionVector accel = pImu->getLinearAcceleration();
            FusionEuler euler = pImu->quaternionToEuler(pImu->getQuaternion());
            cout << setprecision(3);
            cout <<  
                "Linear Accel:\t " <<  setw(10) << accel.axis.x <<  setw(10) << accel.axis.y <<  setw(10) << accel.axis.z << "\n" << 
                "Orientation:\t " << setw(10) << euler.angle.roll << setw(10) << euler.angle.pitch << setw(10) << euler.angle.yaw << endl;

            cout << "\x1b[A\x1b[A"; //Go back up 6 lines
        }
    }
}
/*
// Scaling example from sample code:
fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
*/
