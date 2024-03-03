#include "bullet.hpp"
#include "defines.hpp"
#include <signal.h>

int main(int argc, char *argv[]){
    // Frame buffer shared between graphics engine and polar display
    Frame *pFrame = new Frame(MOTOR_SWEEP_STEPS, LED_STRING_PIXELS);
    Bullet* bullet = new Bullet(0, MOTOR_SWEEP_STEPS, LED_STRING_PIXELS, pFrame);

    // signal(SIGINT, [](int signum){bullet.stop(signum);});
    bullet->start(argc, argv);

    delete bullet;
    delete pFrame;
    
}