#include "../bargraph.hpp"
#include "unistd.h"
#include <math.h>

int main(int argc, char* argv[]){
    Bargraph *pBargraph = new Bargraph();
    double theta = 0.0;
    double value = 0.0;
    while(1){
        value = 5 * sin(theta);
        pBargraph->setValue((int)round(value));
        theta += M_PI / 1000.0;
        if(theta > M_PI) theta = -M_PI;
        if(theta < -M_PI) theta = M_PI;
        sleep(0.2);
    }
}