#include "../polar.hpp"
int main(int argc, char* argv[] ){
    Polar* pPolar = new Polar(-MOTOR_SWEEP_STEPS/2, MOTOR_SWEEP_STEPS/2, LED_STRING_PIXELS);

    pPolar->incrementHeading(-2.25);
    int result = pPolar->getHeadingVariance(HEADING_BUCKETS/2, 22);

}