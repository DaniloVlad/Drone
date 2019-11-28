#include <iostream>
#include <pigpio.h>
#include "include/Drone.h"

int main() {
    if(gpioInitialise() < 0) {
        std::cout << "Error starting pigpio: Run sudo main and if problem still presists run sudo killall pigpio" << std::endl;
        return -1;
    }
    else {
        std::cout << "Starting drone... " << std::endl;
        Drone *drone = new Drone();
        drone -> startDrone();

    }
}