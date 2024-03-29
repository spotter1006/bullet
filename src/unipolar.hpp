#ifndef _UNIPOLAR_H
#define _UNIPOLAR_H
#include <gpiod.hpp>
#include <thread>
#include <mutex>

using namespace std;

class Unipolar{
    public:
        Unipolar();
        ~Unipolar();
        void step();
        inline bool isKeepRunning(){return m_fKeepRunning;}
        void getPosition(int &position);
        void setTargetPosition(int position);
        void setTargetPosition(float degrees);
    private:
        gpiod::line m_line1;
        gpiod::line m_line2;
        gpiod::line m_line3;
        gpiod::line m_line4;
        const unsigned char m_stepTable[4];
        int m_nStepIndex;
        bool m_fKeepRunning;
        thread m_thread;
        int m_nPosition;
        int m_nTargetPosition;
        timed_mutex m_mutex;
};
#endif