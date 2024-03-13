#include "imu.hpp"

#include <thread>
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
				printf("%d baud find sensor\r\n\r\n", bauds[i]);
				return ;
			}
			iRetry--;
		}while(iRetry);		
	}
	printf("can not find sensor\r\n");
	printf("please check your connection\r\n");
}

void Imu::start(){
	m_thread = thread([](Imu* pImu){

		float fAcc[3], fGyro[3], fAngle[3];
		int i , ret;
		unsigned char cBuff[1];	
			
		pImu->AutoScanSensor(IMU_SERIAL_PORT);
		while(pImu->isKeepRunning())
		{
			while(read(fd, cBuff, 1))
			{
				WitSerialDataIn(cBuff[0]);
			}

			usleep(500000);
			
			if(s_cDataUpdate)
			{
				for(i = 0; i < 3; i++)
					{
						fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
						fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
						fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
					}

				if(s_cDataUpdate & ACC_UPDATE)
					{
					// printf("acc:%.3f %.3f %.3f\r\n", fAcc[0], fAcc[1], fAcc[2]);
					s_cDataUpdate &= ~ACC_UPDATE;
					}
				if(s_cDataUpdate & GYRO_UPDATE)
					{
					// printf("gyro:%.3f %.3f %.3f\r\n", fGyro[0], fGyro[1], fGyro[2]);
					s_cDataUpdate &= ~GYRO_UPDATE;
					}
				if(s_cDataUpdate & ANGLE_UPDATE)
					{
					// printf("angle:%.3f %.3f %.3f\r\n", fAngle[0], fAngle[1], fAngle[2]);
					s_cDataUpdate &= ~ANGLE_UPDATE;
					}
				if(s_cDataUpdate & MAG_UPDATE)
					{
					// printf("mag:%d %d %d\r\n", sReg[HX], sReg[HY], sReg[HZ]);
					s_cDataUpdate &= ~MAG_UPDATE;
					}
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


