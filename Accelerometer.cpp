#include "include/Accelerometer.h"

Accelerometer::Accelerometer() {
    this -> i2c_addr = I2C_ADDR_ACC;
    this -> i2c_bus = I2C_BUS;
    this -> data_pin = I2C_SDA;
    this -> clock_pin = I2C_SCL;

    if(gpioSetMode(this -> data_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }

    if(gpioSetMode(this -> clock_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }

    if((this -> i2c_handle = i2cOpen(this -> i2c_bus, this -> i2c_addr, 0)) == (PI_BAD_I2C_BUS | PI_BAD_I2C_ADDR | PI_BAD_FLAGS | PI_NO_HANDLE | PI_I2C_OPEN_FAILED)) {
        perror("Couldn't initialize i2c device");
        exit(EXIT_FAILURE);
    }
}
Accelerometer::Accelerometer(int i2c_addr, int i2c_bus,int data_pin, int clock_pin) {
    if(i2c_bus != 0 || i2c_bus != 1) {
        perror("I2C bus must be either 0 or 1");
    }

    this -> i2c_addr = i2c_addr;
    this -> i2c_bus = i2c_bus;
    this -> data_pin = data_pin;
    this -> clock_pin = clock_pin;

    //ALT0 pg 102 "BCM ARM PERIPHERALS"
    if(gpioSetMode(this -> data_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }

    if(gpioSetMode(this -> clock_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }

    if((this -> i2c_handle = i2cOpen(this -> i2c_bus, this -> i2c_addr, 0)) == (PI_BAD_I2C_BUS | PI_BAD_I2C_ADDR | PI_BAD_FLAGS | PI_NO_HANDLE | PI_I2C_OPEN_FAILED)) {
        perror("Couldn't initialize i2c device");
        exit(EXIT_FAILURE);
    }
}

Accelerometer::~Accelerometer() {
    i2cClose(this -> i2c_handle);
}

int Accelerometer::getGyroX() {
    return (this -> gyroX = i2cReadWordData(this -> i2c_handle, GYRO_X_H));
}

int Accelerometer::getGyroY() {
    return (this -> gyroX = i2cReadWordData(this -> i2c_handle, GYRO_Y_H));
}

int Accelerometer::getGyroZ() {
    return (this -> gyroX = i2cReadWordData(this -> i2c_handle, GYRO_Z_H));
}

int Accelerometer::getAccX() {
    //reads 16-bit word starting at register
    return (this -> accX = i2cReadWordData(this -> i2c_handle, ACC_X_H));
}

int Accelerometer::getAccY() {
    //reads 16-bit word starting at register
    return (this -> accX = i2cReadWordData(this -> i2c_handle, ACC_Y_H));
}

int Accelerometer::getAccZ() {
    //reads 16-bit word starting at register
    return (this -> accX = i2cReadWordData(this -> i2c_handle, ACC_Z_H));
}

int *Accelerometer::getGyroXYZ() {

}

int *Accelerometer::getAccXYZ() {

}

