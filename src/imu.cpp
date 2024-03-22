#include "imu.hpp"

#include <thread>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "wit_c_sdk.h"
using namespace std;

int fd;
char s_cDataUpdate = 0;

void Imu::AutoScanSensor(char* dev)
{
	int bauds[5] = {9600 , 19200 , 38400 , 57600 , 115200};
	int i, iRetry;
	unsigned char cBuff[1];
	
	for(i = 5; i >= 0; i--)
	{
		close(fd);
		serial_open(dev , bauds[i]);
		
		iRetry = 2;
		do
		{
			s_cDataUpdate = 0;
			WitReadReg(AX, 3);
			usleep(200000);
			while(read(fd, cBuff, 1))
			{
				WitSerialDataIn(cBuff[0]);
			}
			if(s_cDataUpdate != 0)
			{
				//cout << "using IMU sensor at " << bauds[i] << " baud" << endl;
				return ;
			}
			iRetry--;
		}while(iRetry);		
	}
	cout << "IMU sensor not detected" << endl;

}

void Imu::start(){
	m_thread = thread([](Imu* pImu){

		unsigned char cBuff[1];	
			
		pImu->AutoScanSensor(IMU_SERIAL_PORT);
		while(pImu->isKeepRunning())
		{
			while(read(fd, cBuff, 1))
			{
				WitSerialDataIn(cBuff[0]);
			}

			usleep(IMU_SAMPLE_INTERVAL_US);	
			
			if(s_cDataUpdate){
				pImu->addMeasurements(s_cDataUpdate);
			}

		}
		
		close(fd);
		return 0;

    }, this);
    m_thread.detach();
}
void Imu::stop(){
	m_fKeepRunning = false;
	// m_thread.join();
}

void Imu::addMeasurements(uint flags){

// auto finish = std::chrono::high_resolution_clock::now();
// std::chrono::duration<double, std::milli> elapsed = finish - start;

	m_mutex.lock();
	if(flags & ACC_UPDATE && flags & GYRO_UPDATE && flags & MAG_UPDATE){		// Wait till all three are ready
		// Wit library stores in double, fusion values take float 
		m_accel.axis.x = sReg[AX] / ACCEL_PER_G; 
		m_accel.axis.y = sReg[AY] / ACCEL_PER_G; 
		m_accel.axis.z = sReg[AZ] / ACCEL_PER_G;
		m_gyro.axis.x = sReg[GX] / GYRO_PER_DEGREES_PER_SECOND; 
		m_gyro.axis.y = sReg[GY] / GYRO_PER_DEGREES_PER_SECOND; 
		m_gyro.axis.z = sReg[GZ] / GYRO_PER_DEGREES_PER_SECOND;
		m_mag.axis.x = 	sReg[HX] / ANGLE_PER_DEGREE; 
		m_mag.axis.y = sReg[HY] / ANGLE_PER_DEGREE; 
		m_mag.axis.z = sReg[HZ] / ANGLE_PER_DEGREE;


 		// Apply calibration
        m_gyro = FusionCalibrationInertial(m_gyro, gyroscopeMisalignment, gyroscopeSensitivity, gyroscopeOffset);
        m_accel = FusionCalibrationInertial(m_accel, accelerometerMisalignment, accelerometerSensitivity, accelerometerOffset);
        m_mag = FusionCalibrationMagnetic(m_mag, softIronMatrix, hardIronOffset);

        // Update gyroscope offset correction algorithm
        m_gyro = FusionOffsetUpdate(&offset, m_gyro);

		auto elapsed = chrono::high_resolution_clock::now() - m_lastImuUpdate;
		auto deltaTime = chrono::duration_cast<std::chrono::duration<float>>(elapsed);

		// Update gyroscope AHRS algorithm
        FusionAhrsUpdate(&m_fusion, m_gyro, m_accel, m_mag, deltaTime.count());	
		m_lastImuUpdate = chrono::high_resolution_clock::now();

		s_cDataUpdate &= (~ACC_UPDATE & ~GYRO_UPDATE & ~MAG_UPDATE) ;

	} 

 		

	if(flags & ANGLE_UPDATE){ 			// This is from the Wit internal fusion stuff, ignore for now
		s_cDataUpdate &= ~ANGLE_UPDATE;
	}	
	if(flags & BIAS_UPDATE){
		for(int i = 0; i < 9; i++){
			m_biasTable[i] = sReg[i + AXOFFSET];
		}
		s_cDataUpdate &= ~BIAS_UPDATE;
	}
	if(flags & SETTTINGS_UPDATE){
		m_settings[0] = sReg[RSW];
		m_settings[1] = sReg[RRATE];
		m_settings[2] = sReg[BAUD];
		m_settings[3] = sReg[AXIS6];
		m_settings[4] = sReg[BANDWIDTH];
		s_cDataUpdate &= ~SETTTINGS_UPDATE;
	}
	m_mutex.unlock();
}

void Imu::getLinearAcceleration(FusionVector &linearAccel){
	m_mutex.lock();
	linearAccel = FusionAhrsGetLinearAcceleration(&m_fusion);
	m_mutex.unlock();

}

void Imu::readBiases(){
	witWriteReg(KEY, KEY_UNLOCK);
	usleep(200000);
	for(int i = 0; i < 9; i++)
	{
		witReadReg(i + AXOFFSET, 2);
		usleep(200000);
	}

}
void Imu::getBiasTable(vector<uint16_t> &offsets){
	m_mutex.lock();
	offsets = m_biasTable;
	m_mutex.unlock();
}
void Imu::readSettings(){
	witWriteReg(KEY, KEY_UNLOCK);
	usleep(200000);
	witReadReg(RSW, 2);
	usleep(200000);
	witReadReg(RRATE, 2);
	usleep(200000);
	witReadReg(BAUD, 2);
	usleep(200000);
	witReadReg(AXIS6, 2);
	usleep(200000);
	witReadReg(BANDWIDTH, 2);
	usleep(200000);

}
void Imu::getSettings(vector<uint16_t> &settings){
	m_mutex.lock();
	settings = m_settings;
	m_mutex.unlock();
}
void Imu::setAxis6(int val){
	WitWriteReg(KEY, KEY_UNLOCK);
	usleep(20000);
	WitWriteReg(AXIS6, val);
	usleep(20000);
}
void Imu::setBandwidth(int bw){
	WitWriteReg(KEY, KEY_UNLOCK);
	usleep(20000);
	WitWriteReg(BANDWIDTH, bw);
	usleep(20000);
}
int Imu::serial_open(const char *dev, int baud){

    fd = open(dev, O_RDWR|O_NOCTTY); 
    if (fd < 0) return fd;

    struct termios newtio,oldtio; 
    if (tcgetattr( fd,&oldtio) != 0) 
      {  
          perror("SetupSerial 1");
	  printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr(fd,&oldtio)); 
          return -1; 
      } 
    bzero( &newtio, sizeof( newtio ) ); 
    newtio.c_cflag  |=  CLOCAL | CREAD;  
    newtio.c_cflag |= CS8; 
    newtio.c_cflag &= ~PARENB; 

    switch( baud ) 
     { 
       case 2400: 
         cfsetispeed(&newtio, B2400); 
         cfsetospeed(&newtio, B2400); 
         break; 
       case 4800: 
         cfsetispeed(&newtio, B4800); 
         cfsetospeed(&newtio, B4800); 
         break; 
       case 9600: 
         cfsetispeed(&newtio, B9600); 
         cfsetospeed(&newtio, B9600); 
         break; 
       case 115200: 
         cfsetispeed(&newtio, B115200); 
         cfsetospeed(&newtio, B115200); 
         break; 
       case 230400: 
         cfsetispeed(&newtio, B230400); 
         cfsetospeed(&newtio, B230400); 
         break; 
       case 460800: 
         cfsetispeed(&newtio, B460800); 
         cfsetospeed(&newtio, B460800); 
         break; 
       default: 
         cfsetispeed(&newtio, B9600); 
         cfsetospeed(&newtio, B9600); 
         break; 
      } 
     newtio.c_cflag &=  ~CSTOPB; 
     newtio.c_cc[VTIME]  = 0; 
     newtio.c_cc[VMIN] = 0; 
     tcflush(fd,TCIFLUSH); 

     if((tcsetattr(fd,TCSANOW,&newtio))!=0) 
       { 
          perror("com set error"); 
          return -1; 
       }
	return 0;
}

