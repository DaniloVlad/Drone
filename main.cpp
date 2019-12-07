/**
*@class main - initates a drone object for operation
*@include - include/Drone.h
*@include - <pigpio.h>
*@include - <iostream>
*/

#include <iostream>
#include <pigpio.h>
#include "include/Drone.h"


int main() {
    std::cout << "Please ensure include/Configurations.h is correctly setup!" << std::endl;
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
