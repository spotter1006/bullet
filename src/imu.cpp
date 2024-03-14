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
				cout << "using IMU sensor at " << bauds[i] << " baud" << endl;
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

			usleep(100000);						// TODO:constant
			ageTick++;
			if(ageTick % 100 == 0)				// TODO:constant
				pImu->decrementHistograms(1);	// TODO:constant
			
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
		double dHeading = atan2(sReg[HY], sReg[HX]);	
		if(dHeading < 0) dHeading += M_PI;
		int nHeading = (int)(dHeading * 254.6479); 		// TODO: constant to turn radians into motor steps
		m_headingHistogram[nHeading] = m_headingHistogram[nHeading] + 1;		// Increment the bucket
		flags &= ~MAG_UPDATE;
	} 

}
void Imu::decrementHistograms(int dec){
	transform(m_headingHistogram.begin(), m_headingHistogram.end(), m_headingHistogram.begin(), 
	[dec](int val){
		return (val >= dec)? val-dec:0;
	});
}

int Imu::getHeadingChange(int heading, int window){
	// int samples = accumulate(m_headingHistogram[heading - window/2], m_headingHistogram[heading + window/2], 0);	
	int samples = 0;
	for(int i = heading - window/2; i < heading + window/2; i++){
		samples += m_headingHistogram[i];
	}	
	double dTotal = 0;

	// Weighted average of histogram in the window
	for(int i = heading - window/2; i < heading + window/2; i++){
		dTotal += ((double)m_headingHistogram[i] / (double)samples) * i;	// Keep precision for accumulation	
	}

	int average = (int)(dTotal / samples);
	return average - heading;
}

int Imu::serial_open(const char *dev, int baud){

    fd = open(dev, O_RDWR|O_NOCTTY); 
    if (fd < 0) return fd;
    if(isatty(STDIN_FILENO)==0) 
      {
   	  printf("standard input is not a terminal device\n"); 
      }   
    else 
      {
	  printf("isatty success!\n"); 
      }

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


