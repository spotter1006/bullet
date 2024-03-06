#ifndef _BULLET_H_
#define _BULLET_H_
#include <signal.h>
#include "polar.hpp"


class Bullet{
    public:
        Bullet(int left, int right, int pixels): m_polar(left, right, pixels), m_fStop(false){}
        int start(int argc, char *argv[]);
        inline void stop(int sigNum){m_fStop = (sigNum == SIGINT);}
    private:
        // Declaration order important for initialization. m_polar referenced by m_graphicEngine
        Polar m_polar;
        bool m_fStop;

        void terminal();
};

#endif