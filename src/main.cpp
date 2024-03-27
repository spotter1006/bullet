#include "bullet.hpp"
#include "defines.hpp"
#include <signal.h>

int main(int argc, char *argv[]){
    Bullet* bullet = new Bullet();
    bullet->start(argc, argv);
    bullet->stop(SIGINT);
    delete bullet;
}