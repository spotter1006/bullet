#include "polar.hpp"
#include "defines.hpp"
#include <thread>
#include <signal.h>
#include <thread>
#include <unistd.h>
#include <atomic>
#include <string.h>
#include <algorithm>
#include <numeric>
using namespace std;


void Polar::start(){
    m_threads.emplace_back(thread([](Polar *pPolar){   
        pPolar->setBargraphValue(0);
        while(pPolar->iskeepRunning()){                       
           
            auto wakeTime = chrono::high_resolution_clock::now() + chrono::microseconds(MAIN_LOOP_INTERVAL_US); 
            pPolar->update();
            float headingChange = pPolar->getHeadingChange();
            pPolar->setBargraphValue(.001f / pPolar->getLinearAcceleration().axis.y);
            // pPolar->setChaserInterval(100);

            pPolar->setMotorTargetPosition(headingChange);
            this_thread::sleep_until(wakeTime);

        }
    },this));

   
    m_imu.start();

    // m_chaser.start();
}
void Polar::update(){
    FusionVector accel = getLinearAcceleration();
    FusionEuler orientation = quaternionToEuler(getQuaternion());

    m_headings.push_front(orientation.angle.yaw);
    if(m_headings.size() > HEADING_AVERAGE_SAMPLES) m_headings.resize(HEADING_AVERAGE_SAMPLES);
    m_fHeadingAverage = accumulate(m_headings.begin(), m_headings.end(), 0) / (float)m_headings.size();

    m_accels.push_front(accel.axis.y);
    if(m_accels.size() > ACCEL_AVERAGE_SAMPLES) m_accels.resize(ACCEL_AVERAGE_SAMPLES);
    m_fAccelAverage = accumulate(m_accels.begin(), m_accels.end(), 0) / (float)m_accels.size();     //To disable the compler change warning for passing double, pass -Wno-psabi to the compiler.

    // Reset average on tacks or jibes
    if(fabs(orientation.angle.yaw - m_fHeadingAverage) > TACKING_ANGLE){
        m_fHeadingAverage = orientation.angle.yaw;

    }
    m_fHeadingChange = orientation.angle.yaw - m_fHeadingAverage;
}

void Polar::stop(){
    // m_chaser.stop();

    m_imu.stop();
    m_fkeepRunning = false;
    for(auto& th : m_threads)
        th.join();
    m_threads.clear();
}



