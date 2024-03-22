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

int main(int argc, char* argv[] ){

    Polar* pPolar = new Polar(-MOTOR_SWEEP_STEPS/2, MOTOR_SWEEP_STEPS/2, LED_STRING_PIXELS);
    try{
        
        cout << "Single value variance test ... " ;
        pPolar->incrementHeading(-2.25);
        evaluate(810, pPolar->getHeadingVariance(HEADING_BUCKETS/2, 22));

        cout << "Symetric values histogram test ... ";
        pPolar->incrementHeading(2.25);
        evaluate(800, pPolar->getHeadingVariance(HEADING_BUCKETS/2, 22));



    }catch(string error){
        cout << error << endl;
    }

}

