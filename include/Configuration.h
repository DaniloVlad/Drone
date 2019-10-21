#ifndef _DRONE_CONFIG
#define _DRONE_CONFIG

//pigpio references pins by the BROADCOM numbers

//find address using `sudo i2cdetect -y 1`
#define I2C_ADDR_ACC 0x69

#define I2C_BUS 1 //the i2c bus used

#if I2C_BUS == 1
    #define I2C_SDA 2 //if bus 1 is used SDA is pin 2 and SCL is pin 3
    #define I2C_SCL 3
#else
    #define I2C_SDA 0
    #define I2C_SCL 1
#endif

#define GPIO_MOTOR_1 27
#define GPIO_MOTOR_2 23
#define GPIO_MOTOR_3 22
#define GPIO_MOTOR_4 24

#define MOTOR_PWM_RANGE 2000
#define MOTOR_PWM_FREQUENCY 8000

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include <pigpio.h>

#endif
