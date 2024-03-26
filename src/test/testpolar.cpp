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

#define WINDOW_SIZE 220 // 22 degrees (+/- 11)
int main(int argc, char* argv[] ){
    string line;
    Polar* pPolar = new Polar(LED_STRING_PIXELS);
    pPolar->start();

    cout << "Running. <enter> to stop ..." << endl;
    getline(cin, line);


    try{

       evaluate(pPolar->getChaserInterval(), -100);
    }catch(string error){
        cout << error << endl;
    }

}

