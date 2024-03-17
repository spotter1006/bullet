#include "imu.hpp"

#include <thread>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <iomanip>
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
		int ageTick = 0;
		unsigned char cBuff[1];	
			
		pImu->AutoScanSensor(IMU_SERIAL_PORT);
		while(pImu->isKeepRunning())
		{
			while(read(fd, cBuff, 1))
			{
				WitSerialDataIn(cBuff[0]);
			}

			usleep(IMU_SAMPLE_INTERVAL_US);	
			ageTick++;
			if(ageTick % IMU_LEAK_RATE == 0)				
				pImu->decrementHistograms(1);
			
			if(s_cDataUpdate)
				pImu->addMeasurements(s_cDataUpdate);


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

void Imu::addMeasurements(int flags){
	m_mutex.lock();
	if(flags & ACC_UPDATE){
		double acc = sqrt(sReg[AX] * sReg[AX] + sReg[AY] * sReg[AY]); 
		double accAngle = atan2(sReg[AY], sReg[AX]);
		// TODO: store these...
		flags &= ~ACC_UPDATE;
	} 
	if(flags & GYRO_UPDATE){
		s_cDataUpdate &= ~GYRO_UPDATE;
	}
	if(flags & ANGLE_UPDATE){ 

		s_cDataUpdate &= ~ANGLE_UPDATE;	
	}				
	if(flags & MAG_UPDATE){
		double dHeading = atan2(sReg[HY], sReg[HX]);								// -pi to pi radians					
		m_nHeading = (int)(dHeading * STEPS_PER_RAD + 0.5); 						// Scale to histogram size and round to nearset integer  
		m_headingHistogram[m_nHeading + HEADING_0_BUCKET]++;						// Increment the bucket
		
		flags &= ~MAG_UPDATE;
	} 
	m_mutex.unlock();
}
void Imu::decrementHistograms(int dec){
	transform(m_headingHistogram.begin(), m_headingHistogram.end(), m_headingHistogram.begin(), 
	[dec](int val){
		return (val >= dec)? val-dec:0;
	});
}

int Imu::getHeadingChange(int window){
	m_mutex.lock();
	long samples = 0;
	long sum = 0;

	int mid = m_nHeading + HEADING_0_BUCKET; 
	int left = mid - window / 2;
	if(left < 0) left += HEADING_BUCKETS;

	vector<int>::iterator it = m_headingHistogram.begin() + left;

	for(int i = 0; i < window; i++){
		int angle = distance(m_headingHistogram.begin(), it) - HEADING_0_BUCKET;
		samples += *it;
		sum += angle * *it;

		if(it == m_headingHistogram.end())
			it = m_headingHistogram.begin();
		else
			it++;
	}	
	m_mutex.unlock();
	int average = samples > 0? sum / samples : m_nHeading;
	return m_nHeading - average;
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

/*
// Scaling example from sample code:
fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
*/
void Imu::displayData(){
	int16_t lReg[144];
	m_mutex.lock();
	memcpy(lReg, sReg, 144 * sizeof(int16_t));
	m_mutex.unlock();

	double compass = (atan2(lReg[HY], lReg[HX]) + M_PI) * 180.0 / M_PI;

	double accelMag = sqrt(lReg[AX]*lReg[AX] + lReg[AY]*lReg[AY]);
	double accelDir = (atan2(lReg[AY], lReg[AX]) + M_PI) * 180.0 / M_PI;

	cout << "Accel:\t" 	<< setw(10) << lReg[AX] << setw(10)  << lReg[AY] << setw(10)  << lReg[AZ] << setw(10) << accelMag << " Gs at " << accelDir << " degrees" << endl;
	cout << "Gyro:\t" 	<< setw(10) << lReg[GX] << setw(10)  << lReg[GY] << setw(10)  << lReg[GZ] << endl;
	cout << "Head:\t" 	<< setw(10) << lReg[HX] << setw(10) << lReg[HY] << setw(10)  << lReg[HZ] << setw(10) << compass << " degrees" << endl;
	cout << "Orient:\t" 	<< setw(10) << lReg[Roll] << setw(10)  << lReg[Pitch] << setw(10)  << lReg[Yaw] << endl;

}

