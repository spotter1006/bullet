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


Polar::Polar(int left, int right, int radius):
    m_nLeftSweepLimit(left),
    m_nRightSweepLimit(right),
    m_nChaserInterval(100),
    m_nAngle(0),
    m_chaser(radius),
    m_intensities({0, 0, 3, 7, 27, 35, 58, 89, 129, 180}),      // Gamma corrected ramp
    m_colors(radius, BLUE), 
    m_imu(),
    m_yAccels(),
    m_headings(),
    m_fCurrentHeading(0),
    m_fKeepSweeping(true)
{
        m_chaser.setPattern(m_intensities, m_colors);
        // Initialize map of 1/10 degree fixed point integer heading histogram buckets over +/- 180 degrees. Initialze the sums to 0
        for(int i = -1800; i <= 1800; i++){
            m_headings.insert(make_pair(i, 0));
        }
}

void Polar::start(){

    m_threads.emplace_back(thread([](Polar *pPolar){   
        unsigned int timeTick = 0;
        while(pPolar->isKeepSweeping()){                       
           
            auto wakeTime = chrono::high_resolution_clock::now() + chrono::microseconds(MAIN_LOOP_INTERVAL_US); 
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
                */
            
                pPolar->addHeading(orientation.angle.yaw);              // Increment corresponding histogram bucket           
                if(timeTick % IMU_LEAK_RATE == 0){
                    pPolar->decrementHistogram();                        // Age out older entries
                }

                float variance = pPolar->getHeadingVariance(HEADING_AVERAGE_WINDOW_STEPS);
                pPolar->setAngle(variance / ANGLE_PER_DEGREE);

                // pPolar->setAngle(orientation.angle.yaw * STEPS_PER_DEGREE);
                
            }
            timeTick++;
            pPolar->setMotorTargetPosition(pPolar->getAngle());
            this_thread::sleep_until(wakeTime);

        }
    },this));
    m_imu.start();
    m_stepper.start();
    m_chaser.start();
}
float Polar::getHeadingVariance(int widthTenthDegrees){
    float sum = 0;
    unsigned int samples = 0;
    int center = round(m_fCurrentHeading * 10.0f);
    int halfWidth = widthTenthDegrees / 2;

    // center going left
    int count = halfWidth;
    int i = center;
    while(count){ 
        samples += m_headings[i];
        sum += ((float)i) * ((float)m_headings[i]);
        if(i == 0) i = HEADING_BUCKETS - 1;  
        else i--; 
        count--;
    }
    
    // Center + 1 going right
    count = halfWidth;
    i = center + 1;
    while(count){   
        samples += m_headings[i];
        sum += ((float)i) * ((float)m_headings[i]);
        if(i == HEADING_BUCKETS - 1) i = 0;
        else  i++; 
        count--;
    }


    if(sum == 0) return 0;
    float average = (sum / samples) / 10.0f;
    return round((m_fCurrentHeading - average));

}
float Polar::getAverageYAccel(){
    float sum = accumulate(m_yAccels.begin(), m_yAccels.end(), 0);
   return  sum / m_yAccels.size();
}
void Polar::addHeading(float heading){
    m_fCurrentHeading = heading;
    float tenths = heading * 10;
    int roundedTenth = round(tenths);
    m_headings[roundedTenth]++;                                  // Increment histogram bucket
}
void Polar::addAccel(float accel){
    m_yAccels.push_front(accel);
    m_yAccels.resize(ACCEL_SAMPLES);
 }

 void Polar::clearHistory(){
    for_each(m_headings.begin(), m_headings.end(), [](pair<const int, unsigned int>& p) {
        p.second = 0;
    });
    transform(m_yAccels.begin(), m_yAccels.end(), m_yAccels.begin(), [](float val){return 0.0;});
 }
void Polar::stop(){
    m_chaser.stop();
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

void Polar::decrementHistogram(){
    for_each(m_headings.begin(), m_headings.end(), [](pair<const int, unsigned int>& p) {
        p.second = p.second - 1;
    });
}