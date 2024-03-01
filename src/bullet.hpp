#ifndef _BULLET_H_
#define _BULLET_H_
#include "polar.hpp"
#include "graphicEngine.hpp"
class Bullet{
    public:
        Bullet(int frames, int bars, int pixels) : m_graphicEngine(frames, bars, pixels){m_fStop = false;}
        ~Bullet();
        int main(int argc, char *argv[]);
        void stop(int sigNum);
    private:
        Polar m_polar;
        GraphicEngine m_graphicEngine;
        bool m_fStop;

};
#endif