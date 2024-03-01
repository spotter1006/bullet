#include "bullet.hpp"
#include "defines.hpp"
#include <signal.h>

Bullet bullet(ANIMATION_FRAMES, MOTOR_SWEEP_STEPS, LED_STRING_PIXELS);

int main(int argc, char *argv[]){
    signal(SIGINT, [](int signum){bullet.stop(signum);});
    bullet.main(argc, argv);
}