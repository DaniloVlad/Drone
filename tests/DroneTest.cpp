#include <pigpio.h>
#include "../include/Drone.h"


int main() {
    if(gpioInitialise() < 0) {
        printf("Couldn't Start pigpio");
        return -1;
    }
    Drone *drone = new Drone();
    drone -> startDrone();
    return 0;
}