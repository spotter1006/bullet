#ifndef _MOTION_SIMULATOR_H_
#define _MOTION_SIMULATOR_H_
#include <vector>
#include <cstdlib>
#include <math.h>
#include <bits/stdc++.h>

#define MAX_TIME 4294967295
using namespace std;

class MotionSimulator{
    public:
        MotionSimulator(double meanAngle, double meanMagnitude, double randomness, double shiftRate, int angleBins, int magnitudeBins, int overSample, int decay):
        m_dMeanAngle(meanAngle),
        m_dMeanMagnitude(meanMagnitude),
        m_dRandomness(randomness),
        m_dAngle(meanAngle),
        m_dMagnitude(meanMagnitude),
        m_dShiftRate(shiftRate),
        m_nTime(0),
        m_angleHistogram(angleBins, 0),
        m_magnitudeHistogram(magnitudeBins, 0),
        m_nOverSample(overSample),
        m_nDecay(decay),
        m_nSamples(0)      
        {srand(1);}        

        void step();
        inline vector<int>& getAngleHistogram(){return m_angleHistogram;}
        inline vector<int>& getMagnitudeHistogram(){return m_magnitudeHistogram;}
    private:
        double m_dMeanAngle;
        double m_dMeanMagnitude;
        double m_dRandomness;
        double m_dAngle;
        double m_dMagnitude;
        double m_dShiftRate;
        unsigned int m_nTime;
        vector<int> m_angleHistogram;
        vector<int> m_magnitudeHistogram;
        int m_nDecay;
        int m_nOverSample;
        unsigned long m_nSamples;
};
#endif