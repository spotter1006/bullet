#ifndef _BULLET_H_
#define _BULLET_H_
#include <signal.h>
#include "polar.hpp"
#include "imu.hpp"


class Bullet{
    public:
        Bullet(int left, int right, int pixels): 
        m_polar(left, right, pixels), 
        m_imu(),
        m_fStop(false){}

        int start(int argc, char *argv[]);
        inline void stop(int sigNum){m_fStop = (sigNum == SIGINT);}
    private:
        Polar m_polar;
        Imu m_imu;
        bool m_fStop;
        void terminal();
};

#endif