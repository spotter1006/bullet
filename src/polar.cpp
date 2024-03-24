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

atomic_flag fWaitForTick;   

Polar::Polar(int left, int right, int radius):
    m_nLeftSweepLimit(left),
    m_nRightSweepLimit(right),
    m_nChaserInterval(100),
    m_nAngle(0),
    m_chaser(radius),
    m_intensities({0, 0, 3, 7, 27, 35, 58, 89, 129, 180}),      // Gamma corrected ramp
    m_colors(radius, BLUE), 
    m_headings(HEADING_BUCKETS,0),
    m_imu(),
    m_yAccels()
{
        signal(SIGALRM, [](int signo){fWaitForTick.clear();});   
        itimerval timer;
        timer.it_interval.tv_usec = timer.it_value.tv_usec = MOTOR_STEP_INTERVAL_US;      
        timer.it_interval.tv_sec = timer.it_value.tv_sec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);
        m_fKeepSweeping = true;
        m_chaser.setPattern(m_intensities, m_colors);
        m_nCurrentHeading=0;
}

void Polar::start(){

    m_threads.emplace_back(thread([](Polar *pPolar){   
        unsigned int timeTick = 0;
        while(pPolar->isKeepSweeping()){                       
            
            // Wait for precise time interval
            while(fWaitForTick.test_and_set()) usleep(2);           
            timeTick++;       
           
            if(pPolar->getChaserInterval() != 0 && timeTick % abs(pPolar->getChaserInterval()) == 0){         
                pPolar->chaserRotate(pPolar->getChaserInterval() < 0 ? -1 : 1);    
            } 

            if(timeTick % IMU_READ_MULTIPLIER == 0){
                FusionVector accel = pPolar->getLinearAcceleration();
                FusionEuler orientation = pPolar->quaternionToEuler(pPolar->getQuaternion());
/*               
                // pPolar->addAccel(accel.axis.y);
                // float acc = pPolar->getAverageYAccel();      TODO:
                float acc =accel.axis.y;
                if(acc == 0 ){
                    pPolar->setChaserInterval(0);
                }else{
                    float interval = 50.0 / acc;
                    int val = 0;
                    if(fabs(interval) > 300){
                        val = signbit(interval)? -300 : 300;
                    }else{
                        val = interval;
                    }
                    
                    pPolar->setChaserInterval(val);
                }
  
            
                pPolar->addHeading(orientation.angle.yaw);              // Increment corresponding histogram bucket           
                if(timeTick % IMU_LEAK_RATE == 0){
                    pPolar->decrementHistogram();                        // Age out older entries
                }

                int variance = pPolar->getHeadingVariance(HEADING_AVERAGE_WINDOW_STEPS);
                pPolar->setAngle(variance);
*/ 
                pPolar->setAngle(orientation.angle.yaw * STEPS_PER_DEGREE);
            }

            pPolar->setMotorTargetPosition(pPolar->getAngle());


        }
    },this));
    m_imu.start();
    m_stepper.start();
}
int Polar::getHeadingVariance(int width){
    long sum = 0;
    long samples = 0;

    int index = m_nCurrentHeading - width/2;
    if(index < 0) index = HEADING_BUCKETS+ index;
    int count = width;
    while(count){
        samples += m_headings[index]; 
        int weight = index_to_angle(index) * m_headings[index];
        sum += weight;
        index = (index + 1) % HEADING_BUCKETS;
        count--;
    }

    if(samples == 0)   return 0;
    int average = sum / samples;

    int diff = index_to_angle(m_nCurrentHeading) - average;
    if(abs(diff) >= 800) diff=0;

    return diff;
}
float Polar::getAverageYAccel(){
    float sum = accumulate(m_yAccels.begin(), m_yAccels.end(), 0);
   return  sum / m_yAccels.size();
}
int Polar::addHeading(float heading){
    int i = (heading + 180.0) * STEPS_PER_DEGREE + 0.5;
    if(i < 0) i = 0;
    m_nCurrentHeading = i;
    if(i > m_headings.size() - 1) i = m_headings.size() - 1;
    m_headings[i]++;
    return i;
 }
 void Polar::addAccel(float accel){
    m_yAccels.push_front(accel);
    m_yAccels.resize(ACCEL_SAMPLES);
 }

 void Polar::clearHistory(){
    transform(m_headings.begin(), m_headings.end(), m_headings.begin(), [](int val){return 0;});
    transform(m_yAccels.begin(), m_yAccels.end(), m_yAccels.begin(), [](float val){return 0.0;});
 }
void Polar::stop(){
    m_stepper.stop();
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
