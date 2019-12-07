#include <iostream>
#include <unistd.h>
#include <time.h>
#include "../include/Accelerometer.h"

int main() {  
    
    int SENS_ACC = 16 * 1000 ;//convert values from mg to g
    int RUNTIME = 60;

    if(gpioInitialise() < 0) {
        std::cout << "Must be run as sudo. Also run sudo killall pigpiod!" << std::endl;
    }
    else {

        Accelerometer *accel = new Accelerometer();
        // create half second delay
        struct timespec sleepTime;
        sleepTime.tv_nsec = 1000000000/8;

        //int read = 0;
        time_t start = time(NULL);

        while((time(NULL) - start) < RUNTIME) {
            int x = accel -> getAccX();    //this returned an int not sure if wanted unsigned short
            int y = accel -> getAccY();
            int16_t z = accel -> getAccZ();
            int gx = accel -> getGyroX();
            int gy = accel -> getGyroY();
            int gz = accel -> getGyroY();
            std::cout << "Acceleration:" << std::endl;
            std::cout << "X = "<< (float) x/SENS_ACC << "g Y ="<< (float) y/SENS_ACC <<"g Z = "<< (float) z/SENS_ACC <<"g " << std::endl;

            std::cout << "Gyro" << std::endl;
            std::cout << "X = " << gx << " Y = " << gy << " Z = " << gz << std::endl;
            sleep(1);
        }
    }
}