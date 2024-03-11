#include "bullet.hpp"
#include <iostream>
#include "polar.hpp"
#include "defines.hpp"

using namespace std;

int Bullet::start(int argc, char *argv[]){
    m_polar.start();
    m_imu.setup(WIT_BAUD_115200, 2);
    m_imu.start();
    terminal();
    m_imu.stop();
    m_polar.stop();
    return 0;
}


// WIT Motion IMU sub menu...
void Bullet::ShowHelp(void)
{
	printf("\r\n************************ Wit Sub-menu **************************************\r\n");
	printf("a   Acceleration calibration.\r\n");
	printf("m   Magnetic field calibration,After calibration send: e to indicate the end\r\n");
	printf("U   Bandwidth increase.\r\n");
	printf("u   Bandwidth reduction.\r\n");
	printf("B   Baud rate increased to 115200.\r\n");
	printf("b   Baud rate reduction to 9600.\r\n");
	printf("R   The return rate increases to 10Hz.\r\n");
	printf("r   The return rate reduction to 1Hz.\r\n");
	printf("C   Basic return content: acceleration, angular velocity, angle, magnetic field.\r\n");
	printf("c   Return content: acceleration.\r\n");
	printf("s   Save parameters\r\n");
	printf("h   help.\r\n");
    printf("q   quit Wit sub-menu\r\n");
	printf("********************************************************************************\r\n");
}
void Bullet::CmdProcess(char s_cCmd)
{
	switch(s_cCmd)
	{
		case 'a':
			if(m_imu.witStartAccCali() != WIT_HAL_OK)
				printf("\r\nSet AccCali Error\r\n");
			break;
		case 'm':
			if(m_imu.witStartMagCali() != WIT_HAL_OK)
				printf("\r\nSet MagCali Error\r\n");
			break;
		case 'e':
			if(m_imu.witStopMagCali() != WIT_HAL_OK)
				printf("\r\nSet MagCali Error\r\n");
			break;
		case 'u':
			if(m_imu.witSetBandwidth(BANDWIDTH_5HZ) != WIT_HAL_OK)
				printf("\r\nSet Bandwidth Error\r\n");
			break;
		case 'U':
			if(m_imu.witSetBandwidth(BANDWIDTH_256HZ) != WIT_HAL_OK)
				printf("\r\nSet Bandwidth Error\r\n");
			break;
		case 'B':
			if(m_imu.witSetUartBaud(WIT_BAUD_115200) != WIT_HAL_OK)
				printf("\r\nSet Baud Error\r\n");
			else
				// close(fd);
				// m_imu.serial_open(IMU_SERIAL_PORT, 115200);

			break;
		case 'b':
			if(m_imu.witSetUartBaud(WIT_BAUD_9600) != WIT_HAL_OK)
				printf("\r\nSet Baud Error\r\n");
			else
				// close(fd);
				// m_imu.serial_open(IMU_SERIAL_PORT, 9600);
			break;
		case 'R':
			if(m_imu.witSetOutputRate(RRATE_10HZ) != WIT_HAL_OK)
				printf("\r\nSet Rate Error\r\n");
			break;
		case 'r':
			if(m_imu.witSetOutputRate(RRATE_1HZ) != WIT_HAL_OK)
				printf("\r\nSet Rate Error\r\n");
			break;
		case 'C':
			if(m_imu.witSetContent(RSW_ACC|RSW_GYRO|RSW_ANGLE|RSW_MAG) != WIT_HAL_OK)
				printf("\r\nSet RSW Error\r\n");
			break;
		case 'c':
			if(m_imu.witSetContent(RSW_ACC) != WIT_HAL_OK)
				printf("\r\nSet RSW Error\r\n");
			break;
		case 's':
			if(m_imu.witSaveParameter() != WIT_HAL_OK)
				printf("\r\nSave parameter Error\r\n");
			break;
		case 'h':
			ShowHelp();
			break;
	}
}
void Bullet:: terminal(){
    string line;
    cout << "Bullet - 'h' for a list of commands" << endl; 
	int angle, interval, hue;
    while(!m_fStop){   
        getline(cin, line);

		switch(line[0]){
			case 'h':
				cout << "***************  Bullet Menu ***************8****" << endl;
				cout << "h             diplay this help message" << endl;
				cout << "q             quit bullet" << endl;
				cout << "z             set 0 angle here" << endl;
				cout << "a<angle>      set the display pointer angle" << endl;
				cout << "i<interval>   set the interval for the chaser" << endl;
				cout << "c<hue>        hue from  (-128 = red to 128 = green)" << endl;
				cout << "w             wit sub-menu" << endl;
				cout << "*************************************************" << endl;
				break;
			case 'q':
				cout << "Quit command recieved, exiting..." << endl;
				m_fStop = true;
            	break;
			case 'z':
				m_polar.zeroAngle();
				cout  << "the current position has been set to 0" << endl;
				break;
			case 'a':
				angle = stoi(line.substr(1));
				cout << "Setting angle to " << angle << endl;
				m_polar.setAngle(angle);      
				break;
			case 'i':
				interval = stoi(line.substr(1));
				cout << "Setting interval to " << interval << endl;
				m_polar.setIterval(interval);
				break;
			case 'c':
				hue = stoi(line.substr(1));
				cout << "Setting color to " << hue << endl;
				m_polar.setHue(hue);
				break;
			case 'w':
			 	ShowHelp();
				while(1){
					getline(cin, line);
					if(line.compare("q") == 0){
						cout << "quitting Wit sub-menu..." << endl;
						break;
					}else CmdProcess(*line.c_str());
				}
				break;
		}





        // if(line.compare("q") == 0){
        //     cout << "Quit command recieved, exiting..." << endl;
        //     break;
        // }else if(line[0] == 'a'){
        //     int angle = stoi(line.substr(1));
        //     cout << "Setting angle to " << angle << endl;
        //     m_polar.setAngle(angle);      
        // }else if(line[0] == 'i'){
        //     int interval = stoi(line.substr(1));
        //     cout << "Setting interval to " << interval << endl;
        //     m_polar.setIterval(interval);
        // }else if(line[0] == 'c'){
        //     int hue = stoi(line.substr(1));
        //     cout << "Setting color to " << hue << endl;
        //     m_polar.setHue(hue);
        // }else if(line.compare("z") == 0){
		// 	m_polar.setAngle(0);
		// 	cout  << "position has been set to 0" << endl;
		// }else if(line.compare("h") == 0){
        //     cout << "***************  Bullet Menu ***************8****" << endl;
        //     cout << "h             diplay this help message" << endl;
        //     cout << "q             quit bullet" << endl;
		// 	cout << "z             set 0 angle here" << endl;
        //     cout << "a<angle>      set the display pointer angle" << endl;
        //     cout << "i<interval>   set the interval for the chaser" << endl;
        //     cout << "c<hue>        hue from  (-128 = red to 128 = green)" << endl;
        //     cout << "w             wit sub-menu" << endl;
		// 	cout << "*************************************************" << endl;
        // }else if(line.compare("w") == 0){
        //     ShowHelp();
        //     while(1){
        //         getline(cin, line);
        //         if(line.compare("q") == 0){
        //             cout << "quitting Wit sub-menu..." << endl;
        //             break;
        //         }else CmdProcess(*line.c_str());
        //     }
        // }
    }
}
