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
int main(int argc, char* argv[] ){

    Polar* pPolar = new Polar(-MOTOR_SWEEP_STEPS/2, MOTOR_SWEEP_STEPS/2, LED_STRING_PIXELS);
    try{
        
        cout << "Go right test ..." ;
        // Average of 0 degrees
        pPolar->addHeading(-0.25);
        pPolar->addHeading(0.25);
        addHeadings(pPolar, 0.0,100);
        pPolar->addHeading(2.25);       // Heading goes right 10 buckets (2.25 degrees)
        evaluate(10, pPolar->getHeadingVariance(22));

        pPolar->clearHistory();
        cout << "Wrap around test ... ";
        // Average of 0 through ends of histogram buffer
        addHeadings(pPolar, -179.9, 100);
        addHeadings(pPolar, 179.9, 100);
        evaluate(0, pPolar->getHeadingVariance(22));

        pPolar->clearHistory();
        cout << "Left right test ..." << endl;
        addHeadings(pPolar, -60.0, 100);
        addHeadings(pPolar, 60.0, 100);
        cout << "left: ";
        evaluate(0, pPolar->getHeadingVariance(22));
        cout << "right:";
        evaluate(0, pPolar->getHeadingVariance(22));



    }catch(string error){
        cout << error << endl;
    }

}

