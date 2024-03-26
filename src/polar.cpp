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

        while(pPolar->iskeepRunning()){                       
           
            auto wakeTime = chrono::high_resolution_clock::now() + chrono::microseconds(MAIN_LOOP_INTERVAL_US); 

            FusionVector accel = pPolar->getLinearAcceleration();
            FusionEuler orientation = pPolar->quaternionToEuler(pPolar->getQuaternion());


            // pPolar->setChaserInterval(100);
            
            pPolar->setMotorTargetPosition(20 * STEPS_PER_DEGREE);
            this_thread::sleep_until(wakeTime);

        }
    },this));

   
    m_imu.start();
    m_stepper.start();
    m_chaser.start();
}


void Polar::stop(){
    m_chaser.stop();
    m_stepper.stop();
    m_imu.stop();
    m_fkeepRunning = false;
    for(auto& th : m_threads)
        th.join();
    m_threads.clear();
}



