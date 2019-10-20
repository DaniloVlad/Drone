/**
 * Accelerometer Header
 * */
#ifndef Accelerometer_H
#define Accelerometer_H

#define ACC_X_H 45
#define ACC_X_L 46
#define ACC_Y_H 47
#define ACC_Y_L 48
#define ACC_Z_H 49
#define ACC_Z_L 50
#define GYRO_X_H 51
#define GYRO_X_L 52
#define GYRO_Y_H 53
#define GYRO_Y_L 54
#define GYRO_Z_H 55
#define GYRO_Z_L 56

#include <iostream>
#include <pigpio.h>
#include "Configuration.h"

class Accelerometer{
    private:
        int data_pin;
        int clock_pin;
        int i2c_handle;
        int i2c_addr;
        int i2c_bus;
        uint16_t accX;
        uint16_t accY;
        uint16_t accZ;
        uint16_t gyroX;
        uint16_t gyroY;
        uint16_t gyroZ;
    public:
        Accelerometer();
        Accelerometer(int i2c_addr, int i2c_bus, int data_pin,int clock_pin);
        ~Accelerometer();

        /** Getters **/
        int getGyroX();
        int getGyroY();
        int getGyroZ();
        int getAccX();
        int getAccY();
        int getAccZ();
        int* getGyroXYZ();
        int* getAccXYZ();

    /** Setters **/
    //nothing right now

};
#endif