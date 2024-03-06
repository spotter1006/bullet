#include "bullet.hpp"
#include "defines.hpp"
#include <signal.h>

int main(int argc, char *argv[]){
    Bullet* bullet = new Bullet(0, 1, LED_STRING_PIXELS);
    bullet->start(argc, argv);
    delete bullet;
}