#include "Accelerometer.h"

int main(){ // wasnt sure if this needed a main to run it.
    int BUS = 1
    const int byte_count = 6;
    unsigned short ADDR = 0x69
    // commented inits for these cuz would be getter'd through accel
    // unsigned short X_ACC = 0x2d
    // unsigned short Y_ACC = 0x2f
    // unsigned short Z_ACC = 0x31
    int SENS_ACC = 16 * 1000 //convert values from mg to g
    int RUNTIME = 60

    if(int pi = gpioInitialise()<0){
        // gpio failed
    }else{

        Accelerometer accel = new Accelerometer();
        // create half second delay
        struct timespec time;
        time.tv_nsec = 1000000000 * (1/2);
        nanosleep(&time, NULL);

        //int read = 0;
        const float start_time = timespec_get(&time, TIME_UTC);

        while((timespec_get(&time, TIME_UTC) - start_time) < RUNTIME){
            int X_ACC = accel.getAccX();    //this returned an int not sure if wanted unsigned short
            char buf[byte_count];
            int bytes_read = i2cReadI2CBlockData(i2c, X_ACC,buf, byte_count) // returns number of bytes read and fills buf
            if (bytes_read >= 0){
                float x = (float) buf[0];  //unsure if this is how buff would be unpacked
                float y = (float) buf[1];
                float z = (float) buf[2];
                std::cout << "X = "<< x/SENS_ACC << "g Y ="<< y/SENS_ACC <<"g Z = "<< z/SENS_ACC <<"g " << std::endl;
                std::cout << "Bytes read: " <<  <<std::endl;
            nanosleep(&time, NULL);
            }
        }
    }
}