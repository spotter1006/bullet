#include "../polar.hpp"
#include <iostream>
#include<sstream>

void evaluate(float expected, float result){
    ostringstream error;
    if(result == expected){
        cout << "pass" << endl;
    }else{
        error << "fail. Expected " << expected << ", got " << result;
        throw(error.str());
    }
}
void addHeadings(Polar* pPolar, float heading, int samples){
    for(int i = 0; i < samples; i++){
        pPolar->addHeading(heading);
    }
}
#define WINDOW_SIZE 220 // 22 degrees (+/- 11)
int main(int argc, char* argv[] ){

    Polar* pPolar = new Polar(-MOTOR_SWEEP_STEPS/2, MOTOR_SWEEP_STEPS/2, LED_STRING_PIXELS);
    try{
        
        cout << "Go right test ..." ;
        pPolar->addHeading(-0.5f);
        addHeadings(pPolar, 0.0f, 100);       // 100 headings at 0 degrees
        pPolar->addHeading(0.5f);
        pPolar->setCurrentHeading(10.0f);       // Heading goes right 10 degrees
        evaluate(-10.0f, pPolar->getHeadingVariance(WINDOW_SIZE));

        pPolar->clearHistory();
        cout << "Go left test ..." ;
        addHeadings(pPolar, 0.0f, 100);       // 100 headings at 0 degrees
        pPolar->setCurrentHeading(-10.0f);       // Heading goes left 10 degrees
        evaluate(10.0f, pPolar->getHeadingVariance(WINDOW_SIZE));

        pPolar->clearHistory();
        cout << "Wrap around test ... ";
        // Average of 0 through south heading (180 to -180)
        addHeadings(pPolar, -179.9f, 100);
        addHeadings(pPolar, 179.9f, 100);
        pPolar->setCurrentHeading(0.0f);
        evaluate(0.0f, pPolar->getHeadingVariance(WINDOW_SIZE));

        pPolar->clearHistory();
        cout << "Local lefty righty test ..." << endl;
        addHeadings(pPolar, -60.0f, 100);
        addHeadings(pPolar, 60.0f, 100);
        cout << "left: ";
        pPolar->setCurrentHeading(-60.0f);
        evaluate(0.0f, pPolar->getHeadingVariance(WINDOW_SIZE));
        cout << "right:";
        pPolar->setCurrentHeading(60.0f);
        evaluate(0.0f, pPolar->getHeadingVariance(WINDOW_SIZE));

        pPolar->clearHistory();
        cout << "average test ...";
        addHeadings(pPolar, 5.0f, 100);
        addHeadings(pPolar, 10.0f, 100);
        pPolar->setCurrentHeading(0.0f);
        evaluate(7.5f, pPolar->getHeadingVariance(WINDOW_SIZE));

        pPolar->clearHistory();
        cout << "decrement test ";
        addHeadings(pPolar, 5.0f, 100);
        pPolar->decrementHistogram();
        cout << "positive value ...";
        evaluate(99, pPolar->getHeadingCount(5.0f));
        cout << "zero value ...";
        evaluate(0,  pPolar->getHeadingCount(0.0f));

        pPolar->clearHistory();
        for(int i =0; i < ACCEL_SAMPLES; i++){
            pPolar->addAccel(1.0);
        }
        float result = pPolar->getAverageYAccel();
        cout << "Average accel = " << result << endl;

    }catch(string error){
        cout << error << endl;
    }

}

