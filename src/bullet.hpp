#ifndef _BULLET_H_
#define _BULLET_H_
#include <signal.h>
#include "polar.hpp"
#include "imu.hpp"


class Bullet{
    public:
        Bullet(int pixels): 
        m_polar(pixels),
        m_fStop(false){}

        int start(int argc, char *argv[]);
        inline void stop(int sigNum){m_fStop = (sigNum == SIGINT);}
    private:
        Polar m_polar;

        bool m_fStop;
        void terminal();
        void CmdProcess(char s_cCmd);
        void ShowHelp();
};

#endif