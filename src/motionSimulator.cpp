#include "motionSimulator.hpp"


#define ANGLE_FACTOR 1.0
#define MAGNITUDE_FACTOR 1.0
using namespace std;

void MotionSimulator::step(){
    for(int i =0; i < m_nOverSample; i++){
        m_nSamples++;
        m_dAngle = fmod(m_dAngle + m_dShiftRate * m_nTime + (rand() / RAND_MAX - 0.5) * m_dRandomness, 2.0 * M_PI);
        m_dMagnitude +=  (rand() / RAND_MAX - 0.5) * m_dRandomness;
        m_nTime++;

        // Apply integrations
        int angle = m_dAngle * ANGLE_FACTOR + 0.5;
        if(angle >= m_angleHistogram.size()) angle =  m_angleHistogram.size() - 1;
        m_angleHistogram[angle]++;
        
        int magnitude = m_dMagnitude * MAGNITUDE_FACTOR + 0.5;
        if(magnitude >= m_magnitudeHistogram.size()) magnitude = m_magnitudeHistogram.size() - 1;
        m_magnitudeHistogram[magnitude]++;
    }

    // Apply decays
    if(m_nTime % m_nDecay == 0){
        transform(m_angleHistogram.begin(), m_angleHistogram.end(), m_angleHistogram.begin(), [](int val){return val - 1;});
        transform(m_magnitudeHistogram.begin(), m_magnitudeHistogram.end(), m_magnitudeHistogram.begin(), [](int val){return val - 1;});
    }

    // Re-calculate means
    m_dMeanAngle = accumulate(m_angleHistogram.begin(), m_angleHistogram.end(), 0.0) / m_nSamples;
    m_dMeanMagnitude = accumulate(m_magnitudeHistogram.begin(), m_angleHistogram.end(), 0.0) / m_nSamples;

    m_nTime++;
}