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
        void reset();
        void getPosition(int &position);
        void setPosition(int &position);
        void setTargetPosition(int position);
        void setTargetPosition(float degrees);
        inline int getStepInterval(){return m_nStepInterval;}
    private:
        gpiod::line m_line1;
        gpiod::line m_line2;
        gpiod::line m_line3;
        gpiod::line m_line4;
        const unsigned char m_stepTable[4][4];
        int m_nStepIndex;
        bool m_fKeepRunning;
        thread m_thread;
        int m_nPosition;
        int m_nTargetPosition;
        int m_nStepInterval;
        timed_mutex m_mutex;
        void setOutputs();
};
#endif