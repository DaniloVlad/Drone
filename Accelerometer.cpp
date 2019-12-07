#include "include/Accelerometer.h"

/** Function: Accelerometer
*   @brief - Constructs the accelerometer without any specific paramater input for it's components. 
*/  
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

  	// Attempt's to initialize the i2c device.
    if((this -> i2c_handle = i2cOpen(this -> i2c_bus, this -> i2c_addr, 0)) == (PI_BAD_I2C_BUS | PI_BAD_I2C_ADDR | PI_BAD_FLAGS | PI_NO_HANDLE | PI_I2C_OPEN_FAILED)) {
        perror("Couldn't initialize i2c device");
        exit(EXIT_FAILURE);
    }

    //configure
    if(i2cWriteByteData(this -> i2c_handle, 0x7F, 0) != 0) {
        perror("Couldn't set register bank 0");
        exit(EXIT_FAILURE);
    }

    if(i2cWriteByteData(this -> i2c_handle, 0x06, 0x80) != 0) {
        perror("Couldn't Configure PWR_MGMNT");
        exit(EXIT_FAILURE);
    }
    sleep(2);

    if(i2cWriteByteData(this -> i2c_handle, 0x06, 0) != 0) {
        perror("Couldn't Reset PWR_MGMNT");
        exit(EXIT_FAILURE);
    }
    if(i2cWriteByteData(this -> i2c_handle, 0x7F, 0x20) != 0) {
        perror("Couldn't select register bank 2");
        exit(EXIT_FAILURE);
    }

    if(i2cWriteByteData(this -> i2c_handle, 0x14, 0x07) != 0) {
        perror("Couldn't configure sensitivity!");
        exit(EXIT_FAILURE);
    } 

    if(i2cWriteByteData(this -> i2c_handle, 0x15, 0x03) != 0) {
        perror("Couldn't configure sample averaging!");
        exit(EXIT_FAILURE);
    } 

    if(i2cWriteByteData(this -> i2c_handle, 0x7F, 0) != 0) {
        perror("Couldn't return to register bank 0");
        exit(EXIT_FAILURE);
    }
    
}

/** Function: Accelerometer
*   @brief Constructor for Accelerometer class with specific parameter input.
*   @param i2c_addr - address of i2c device.
*   @param i2c_bus - i2c bus.
*   @param data_pin - SDA pin for i2c device.
*   @param clock_pin - SCL pin for i2c device.
*/  
Accelerometer::Accelerometer(int i2c_addr, int i2c_bus,int data_pin, int clock_pin) {
  
    // Checks if the i2c bus is of the desired type.
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

   	// Attempt's to initialize the i2c device.
    if((this -> i2c_handle = i2cOpen(this -> i2c_bus, this -> i2c_addr, 0)) == (PI_BAD_I2C_BUS | PI_BAD_I2C_ADDR | PI_BAD_FLAGS | PI_NO_HANDLE | PI_I2C_OPEN_FAILED)) {
        perror("Couldn't initialize i2c device");
        exit(EXIT_FAILURE);
    }
    //configure
    if(i2cWriteByteData(this -> i2c_handle, 0x7F, 0) != 0) {
        perror("Couldn't set register bank 0");
        exit(EXIT_FAILURE);
    }

    if(i2cWriteByteData(this -> i2c_handle, 0x06, 0x80) != 0) {
        perror("Couldn't Configure PWR_MGMNT");
        exit(EXIT_FAILURE);
    }
    sleep(2);

    if(i2cWriteByteData(this -> i2c_handle, 0x06, 0) != 0) {
        perror("Couldn't Reset PWR_MGMNT");
        exit(EXIT_FAILURE);
    }
    if(i2cWriteByteData(this -> i2c_handle, 0x7F, 0x20) != 0) {
        perror("Couldn't select register bank 2");
        exit(EXIT_FAILURE);
    }

    if(i2cWriteByteData(this -> i2c_handle, 0x14, 0x07) != 0) {
        perror("Couldn't configure sensitivity!");
        exit(EXIT_FAILURE);
    } 

    if(i2cWriteByteData(this -> i2c_handle, 0x15, 0x03) != 0) {
        perror("Couldn't configure sample averaging!");
        exit(EXIT_FAILURE);
    } 

    if(i2cWriteByteData(this -> i2c_handle, 0x7F, 0) != 0) {
        perror("Couldn't return to register bank 0");
        exit(EXIT_FAILURE);
    }
    
}

/** Function: Accelerometer deconstructor.
*   @brief - Deconstructor method for the accelerometer.
*/  
Accelerometer::~Accelerometer() {
    i2cClose(this -> i2c_handle);
}

/** Function: getGyroX
*   @brief - Getter function for the gyroscope's current X-axis data.
*/  
int Accelerometer::getGyroX() {
    return (this -> gyroX = i2cReadWordData(this -> i2c_handle, GYRO_X_H));
}

/** Function: getGyroY
*   @brief - Getter function for the gyroscope's current Y-axis data. 
*/  
int Accelerometer::getGyroY() {
    return (this -> gyroY = i2cReadWordData(this -> i2c_handle, GYRO_Y_H));
}

/** Function: getGyroZ
*   @brief - Getter function for the gyroscope's current Z-axis data.
*/  
int Accelerometer::getGyroZ() {
    return (this -> gyroZ = i2cReadWordData(this -> i2c_handle, GYRO_Z_H));
}

/** Function: getAccX
*   @brief - Getter function for the accelerometer's current X-axis data.
*/  
int16_t Accelerometer::getAccX() {
    //reads 16-bit word starting at register
    return (this -> accX = i2cReadWordData(this -> i2c_handle, ACC_X_H));
}

/** Function: getAccY
*   @brief - Getter function for the accelerometer's current Y-axis data.
*/  
int16_t Accelerometer::getAccY() {
    //reads 16-bit word starting at register
    return (this -> accY = i2cReadWordData(this -> i2c_handle, ACC_Y_H));
}

/** Function: getAccZ
*   @brief - Getter function for the accelerometer's current Z-axis data.
*/  
int16_t Accelerometer::getAccZ() {
    //reads 16-bit word starting at register
    return (this -> accZ = i2cReadWordData(this -> i2c_handle, ACC_Z_H));
}

/** Function: getGyroXYZ
*   @brief - Getter function for the gyroscope's current X, Y and Z axis data.
*/  
signed short *Accelerometer::getGyroXYZ() {
    signed short *val = new signed short(3);
    val[0] = getGyroX();
    val[1] = getGyroY();
    val[2] = getGyroZ();
    return val;
}

/** Function: getAccXYZ
*   @brief - Getter function for the accelerometer's current X, Y and Z axis data.
*/  
signed short *Accelerometer::getAccXYZ() {
    signed short *val = new signed short(3);
    val[0] = getAccX();
    val[1] = getAccY();
    val[2] = getAccZ();
    return val;
}

