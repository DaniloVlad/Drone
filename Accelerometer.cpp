#include "include/Accelerometer.h"

// Constructs the accelerometer without any specific paramater input for it's components.
Accelerometer::Accelerometer() {
    this -> i2c_addr = I2C_ADDR_ACC;
    this -> i2c_bus = I2C_BUS;
    this -> data_pin = I2C_SDA;
    this -> clock_pin = I2C_SCL;

  	// If the GPIO mode is unable to be set to alternative mode 0, send an error message and exit.
    if(gpioSetMode(this -> data_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }
  
  	// If the GPIO mode is unable to be set to alternative mode 0, send an error message and exit.
    if(gpioSetMode(this -> clock_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }

  	// 
    if((this -> i2c_handle = i2cOpen(this -> i2c_bus, this -> i2c_addr, 0)) == (PI_BAD_I2C_BUS | PI_BAD_I2C_ADDR | PI_BAD_FLAGS | PI_NO_HANDLE | PI_I2C_OPEN_FAILED)) {
        perror("Couldn't initialize i2c device");
        exit(EXIT_FAILURE);
    }
}

// Constructs the accelerometer with specific paramater input for it's components.
/* Function: Accelerometer
*   @breif Constructor for Accelerometer class
*   @param i2c_addr address of i2c device
*   @param i2c_bus i2c bus
*   @param data_pin SDA pin for i2c device
*   @param clock_pin SCL pin for i2c device
*/  
Accelerometer::Accelerometer(int i2c_addr, int i2c_bus,int data_pin, int clock_pin) {
  
    //
    if(i2c_bus != 0 || i2c_bus != 1) {
        perror("I2C bus must be either 0 or 1");
    }

  	// Set components to given parameters.
    this -> i2c_addr = i2c_addr;
    this -> i2c_bus = i2c_bus;
    this -> data_pin = data_pin;
    this -> clock_pin = clock_pin;

  	// If the GPIO mode is unable to be set to alternative mode 0, send an error message and exit.
    if(gpioSetMode(this -> data_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }
  
  	// If the GPIO mode is unable to be set to alternative mode 0, send an error message and exit.
    if(gpioSetMode(this -> clock_pin, PI_ALT0) != 0) {
        perror("Couldn't set mode");
        exit(EXIT_FAILURE);

    }

   	//
    if((this -> i2c_handle = i2cOpen(this -> i2c_bus, this -> i2c_addr, 0)) == (PI_BAD_I2C_BUS | PI_BAD_I2C_ADDR | PI_BAD_FLAGS | PI_NO_HANDLE | PI_I2C_OPEN_FAILED)) {
        perror("Couldn't initialize i2c device");
        exit(EXIT_FAILURE);
    }
}

// Deconstructor method for the accelerometer.
Accelerometer::~Accelerometer() {
    i2cClose(this -> i2c_handle);
}

// Getter function for the gyroscope's current X-axis data.
int Accelerometer::getGyroX() {
    return (this -> gyroX = i2cReadWordData(this -> i2c_handle, GYRO_X_H));
}

// Getter function for the gyroscope's current Y-axis data.
int Accelerometer::getGyroY() {
    return (this -> gyroX = i2cReadWordData(this -> i2c_handle, GYRO_Y_H));
}

// Getter function for the gyroscope's current Z-axis data.
int Accelerometer::getGyroZ() {
    return (this -> gyroX = i2cReadWordData(this -> i2c_handle, GYRO_Z_H));
}

// Getter function for the accelerometer's current X-axis data.
int Accelerometer::getAccX() {
    //reads 16-bit word starting at register
    return (this -> accX = i2cReadWordData(this -> i2c_handle, ACC_X_H));
}

// Getter function for the accelerometer's current Y-axis data.
int Accelerometer::getAccY() {
    //reads 16-bit word starting at register
    return (this -> accX = i2cReadWordData(this -> i2c_handle, ACC_Y_H));
}

// Getter function for the accelerometer's current Z-axis data.
int Accelerometer::getAccZ() {
    //reads 16-bit word starting at register
    return (this -> accX = i2cReadWordData(this -> i2c_handle, ACC_Z_H));
}

// Getter function for the gyroscope's current X, Y and Z axis data.
signed short *Accelerometer::getGyroXYZ() {
    short val[3] = {0};
	val[0] = getGyroX();
  	val[1] = getGyroY();
  	val[2] = getGyroZ();
    return val;
}

// Getter function for the accelerometer's current X, Y and Z axis data.
signed short *Accelerometer::getAccXYZ() {
    short val[3] = {0};
	val[0] = getAccX();
  	val[1] = getAccY();
  	val[2] = getAccZ();
    return val;
}

