#include "../polar.hpp"
#include <iostream>
#include<sstream>

void evaluate(int expected, int result){
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
#define WINDOW_SIZE 220 // 22 degrees
int main(int argc, char* argv[] ){

    Polar* pPolar = new Polar(-MOTOR_SWEEP_STEPS/2, MOTOR_SWEEP_STEPS/2, LED_STRING_PIXELS);
    try{
        
        cout << "Go right test ..." ;
        // Average of 0 degrees
        pPolar->addHeading(-2.5f);
        pPolar->addHeading(2.5f);
        addHeadings(pPolar, 0.0f, 100);       // 100 headings at 0 degrees
        
        pPolar->addHeading(10.0f);       // Heading goes right 10 buckets (2.25 degrees)
        evaluate(10, pPolar->getHeadingVariance(WINDOW_SIZE));

        cout << "Go left test ..." ;
        pPolar->addHeading(-20.0f);       // Heading goes right 10 buckets (2.25 degrees)
        evaluate(-10, pPolar->getHeadingVariance(WINDOW_SIZE));

        pPolar->clearHistory();
        cout << "Wrap around test ... ";
        // Average of 0 through ends of histogram buffer
        addHeadings(pPolar, -179.9f, 100);
        addHeadings(pPolar, 179.9f, 100);
        evaluate(0, pPolar->getHeadingVariance(WINDOW_SIZE));

        pPolar->clearHistory();
        cout << "Local lefty righty test ..." << endl;
        addHeadings(pPolar, -60.0f, 100);
        addHeadings(pPolar, 60.0f, 100);
        cout << "left: ";
        evaluate(0, pPolar->getHeadingVariance(WINDOW_SIZE));
        cout << "right:";
        evaluate(0, pPolar->getHeadingVariance(WINDOW_SIZE));

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

