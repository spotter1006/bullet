#include "imu.hpp"
#include "defines.hpp"
#include <thread>
#include "wit_c_sdk.h"
using namespace std;

static char s_cDataUpdate = 0;

int fd;

void Imu::AutoScanSensor(char* dev)
{
	int i, iRetry;
	unsigned char cBuff[1];
	
	for(i = 1; i < 10; i++)
	{
		serial_close();
		serial_open(dev , m_uiBaud[i]);
		
		iRetry = 2;
		do
		{
			s_cDataUpdate = 0;
			WitReadReg(AX, 3);
			sleep(200);
			while(serial_read_data(cBuff, 1))
			{
				WitSerialDataIn(cBuff[0]);
			}
			if(s_cDataUpdate != 0)
			{
				printf("%d baud find sensor\r\n\r\n", m_uiBaud[i]);
				return ;
			}
			iRetry--;
		}while(iRetry);		
	}
	printf("can not find sensor\r\n");
	printf("please check your connection\r\n");
}
void Imu::SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
    int i;
    for(i = 0; i < uiRegNum; i++)
    {
        switch(uiReg)
        {
//            case AX:
//            case AY:
            case AZ:
				s_cDataUpdate |= ACC_UPDATE;
            break;
//            case GX:
//            case GY:
            case GZ:
				s_cDataUpdate |= GYRO_UPDATE;
            break;
//            case HX:
//            case HY:
            case HZ:
				s_cDataUpdate |= MAG_UPDATE;
            break;
//            case Roll:
//            case Pitch:
            case Yaw:
				s_cDataUpdate |= ANGLE_UPDATE;
            break;
            default:
				s_cDataUpdate |= READ_UPDATE;
			break;
        }
		uiReg++;
    }
}
void Imu::setup(int baudIndex, int updateRate){
	 if((serial_open(IMU_SERIAL_PORT , 9600)<0))
	 {
	     printf("open %s fail\n", IMU_SERIAL_PORT);
	 }
	else printf("open %s success\n", IMU_SERIAL_PORT);
	
	WitRegisterCallBack(SensorDataUpdata);		// TODO: move this to a lambda in the  constructor 

	AutoScanSensor(IMU_SERIAL_PORT);

}
void Imu::start(){
	m_thread = thread([](Imu* pImu){

		float fAcc[3], fGyro[3], fAngle[3];
		int i , ret;
		unsigned char cBuff[1];	
			
		while(pImu->isKeepRunning())
		{
			while(pImu->serial_read_data(cBuff, 1))
			{
				WitSerialDataIn(cBuff[0]);
			}

			sleep(500);
			
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
		
		pImu->serial_close();
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
void Imu::serial_close(){
	close(fd);
}
int Imu::serial_read_data(unsigned char *val, int len){
	int red = read(fd,val,len);
	return red;
}
