#ifndef _GRPAPIC_ENGINE_H_
#define _GRPAPIC_ENGINE_H_
#include <vector>
#include "frame.hpp"
#include <thread>
#include "pattern.hpp"

using namespace std;

class GraphicEngine{
    public:
        GraphicEngine(int nFrames, int bars, int pixels) : m_patterns(){
            m_fKeepRunning = true;
            m_pFrame = NULL;
        };

        void start();
        void stop();
        void paint();
        void display();
        inline bool isKeepRunning(){return m_fKeepRunning;}   
        inline void setFrame(Frame *pFrame){m_pFrame = pFrame;}
    private:
        vector<Pattern> m_patterns;
        Frame *m_pFrame; 
        bool m_fKeepRunning;
        vector<thread> m_threads;
};

#endif