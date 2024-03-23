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
        
        cout << "Go right test ..." ;
        for(int i =0; i <100; i++){
            pPolar->addHeading(0.0);
        }
        pPolar->addHeading(2.25);
        evaluate(10, pPolar->getHeadingVariance(22));

        pPolar->clearHistory();
        cout << "Wrap around test ... ";
        pPolar->addHeading(179.9);
        pPolar->addHeading(-179.9);
        evaluate(0, pPolar->getHeadingVariance(5));

        pPolar->clearHistory();
        cout << "Left right test ...";
        pPolar->addHeading(-60.0);
        pPolar->addHeading(60.0);
        cout << "left: ";
        evaluate(-800, pPolar->getHeadingVariance(5));
        cout << "right:";
        evaluate(-800, pPolar->getHeadingVariance(5));



    }catch(string error){
        cout << error << endl;
    }

}

