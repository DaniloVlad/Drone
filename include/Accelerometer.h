/**
* Accelerometer Header
*/

// Function/Class definitions can be seen in the accelerometer data sheet pdf within the repository.
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
#include "Configuration.h"


class Accelerometer{
  
  	// Defines all the variables that will be used in the construction of the accelerometer object.
    private:
        int data_pin;		// The pin used to connect data.
        int clock_pin;		// The pin used to connect the stream of data for the clock.
        int i2c_handle;
        int i2c_addr;		// Stores the physical address of i2c with a value of 1 for read and 0 for write.
        int i2c_bus;
        uint16_t accX;		// High byte version of accelorometer X-axis data.
        uint16_t accY;		// High byte version of accelorometer Y-axis data.
        uint16_t accZ;		// High byte version of accelorometer Z-axis data.
        uint16_t gyroX;		// High byte version of gyroscope X-axis data.
        uint16_t gyroY;		// High byte version of gyroscope X-axis data.
        uint16_t gyroZ;		// High byte version of gyroscope X-axis data.
  
  	// Accelerometer functions.
    public:
  		// Specifies a base constructor, a constructor with params and a deconstructor function.
        Accelerometer();
        Accelerometer(int i2c_addr, int i2c_bus, int data_pin,int clock_pin);
        ~Accelerometer();

        /** Getter Functions **/
        int getGyroX();
        int getGyroY();
        int getGyroZ();
        int getAccX();
        int getAccY();
        int getAccZ();
        signed short* getGyroXYZ();
        signed short* getAccXYZ();

};
#endif
