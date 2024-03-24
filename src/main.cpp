#include "bullet.hpp"
#include "defines.hpp"
#include <signal.h>

int main(int argc, char *argv[]){
    Bullet* bullet = new Bullet(-MOTOR_SWEEP_STEPS/2, MOTOR_SWEEP_STEPS/2, LED_STRING_PIXELS);
    bullet->start(argc, argv);
    bullet->stop(SIGINT);
    delete bullet;
}