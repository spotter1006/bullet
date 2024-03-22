#include "polar.hpp"
#include "defines.hpp"
#include <thread>
#include <signal.h>
#include <thread>
#include <unistd.h>
#include <atomic>
#include <string.h>
#include <algorithm>
using namespace std;

atomic_flag fWaitForTick;   

Polar::Polar(int left, int right, int radius):
    m_nLeftSweepLimit(left),
    m_nRightSweepLimit(right),
    m_nInterval(100),
    m_nAngle(0),
    m_chaser(radius),
    m_intensities({0, 0, 3, 7, 27, 35, 58, 89, 129, 180}),      // Gamma corrected ramp
    m_colors(radius, BLUE), 
    m_headings(HEADING_BUCKETS,0),
    m_imu()
    {
        signal(SIGALRM, [](int signo){fWaitForTick.clear();});   
        itimerval timer;
        timer.it_interval.tv_usec = timer.it_value.tv_usec = MOTOR_STEP_INTERVAL_US;      
        timer.it_interval.tv_sec = timer.it_value.tv_sec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);
        m_fKeepSweeping = true;
        m_chaser.setPattern(m_intensities, m_colors);
}

void Polar::start(){
    
    m_threads.emplace_back(thread([](Polar *pPolar){   
        unsigned int timeTick = 0;
        while(pPolar->isKeepSweeping()){                       
            while(fWaitForTick.test_and_set()) usleep(2);   // Wait for precise time interval
            timeTick++;       
           
            if(pPolar->getInterval() != 0 && timeTick % abs(pPolar->getInterval()) == 0){         
                pPolar->chaserRotate(pPolar->getInterval() < 0? -1 : 1);    
            } 

            int headingChange = 0;
            FusionVector linearAcceleration = pPolar->getLinearAcceleration();
            FusionEuler orientation = pPolar->quaternionToEuler(pPolar->getQuaternion());
            
            int bucket = pPolar->incrementHeading(orientation.angle.yaw);        // Increment corresponding histogram bucket           
            if(timeTick % IMU_LEAK_RATE == 0){
                pPolar->decrementHistogram();                        // Age out ol;der entries
            }

            int variance = pPolar->getHeadingVariance(bucket, HEADING_AVERAGE_WINDOW_STEPS);
            pPolar->setAngle(variance);

            // Set chaser speed based on linear acceleration ...

            int move = pPolar->getAngle() - pPolar->getMotorPosition();
            if(move > 0){
                if(pPolar->getMotorPosition() < pPolar->getRightSweepLimit()) pPolar->stepMotor(1);
            }else if(move < 0){
                if(pPolar->getMotorPosition() > pPolar->getLeftSweepLimit()) pPolar->stepMotor(-1);
            }


        }
    },this));
    m_imu.start();
}
int Polar::getHeadingVariance(int center, int width){
    long sum = 0;
    long samples = 0;

    int left = center - width /2;
    if(left < 0)
         left = m_headings.size() + left;
    auto it = m_headings.begin() + left;
    int headingVal = left - m_headings.size() / 2;
    for(int i = 0; i < width; i++){
        samples += *it;
        sum += *it * headingVal;

        if(it == m_headings.end()){
            it = m_headings.begin();
            headingVal = -(m_headings.size()/ 2);
        }else{
            it++;
            headingVal++;
        }
    }
    int average = sum / samples;
    return center - average;
    
}
int Polar::incrementHeading(float heading){
    int i = (heading + 180.0) * STEPS_PER_DEGREE + 0.5;
    if(i < 0) i = 0;
    if(i > m_headings.size() - 1) i = m_headings.size() - 1;
    m_headings[i]++;
    return i;
 }
 void Polar::clearHistory(){
    transform(m_headings.begin(), m_headings.end(), m_headings.begin(), [](int val){return 0;});
 }
void Polar::stop(){
    m_imu.stop();
    m_fKeepSweeping = false;
    for(auto& th : m_threads)
        th.join();
    m_threads.clear();
}

void Polar::setHue(int hue){
    m_colors = vector<ws2811_led_t>(m_colors.size(), redToGreen(hue));  // Monotone 
    m_chaser.setPattern(m_intensities, m_colors);
}

void Polar::home(){

    while(m_stepper.getPosition() > m_nLeftSweepLimit * 2){              // Move way past the limit stalling the motor at the limit
        m_stepper.step(-1);
        usleep(MOTOR_STEP_INTERVAL_US * 16);         // 1/16 speed
    }

    for(int i = 0; i < (m_nRightSweepLimit - m_nLeftSweepLimit)/2; i++){       
        m_stepper.step(1);
        usleep(MOTOR_STEP_INTERVAL_US * 16);             
    }

    for(int i = 0; i< 8; i++){
        m_stepper.step(-1);
        usleep(MOTOR_STEP_INTERVAL_US * 16);  
    }

    setAngle(0);
    m_stepper.zeroPosition();
    m_stepper.resetPulse();         // Resets the indexer to electrical angle 45 degrees
}

void Polar::decrementHistogram(){
    transform(m_headings.begin(), m_headings.end(), m_headings.begin(), [](int val){
        return (val > 0)? val -1 : 0;
    });
}
