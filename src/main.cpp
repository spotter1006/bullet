#include "bullet.hpp"
#include "defines.hpp"

Bullet bullet(ANIMATION_FRAMES, MOTOR_SWEEP_STEPS, LED_STRING_PIXELS);

int main(int argc, char *argv[]){
    bullet.main(argc, argv);
}