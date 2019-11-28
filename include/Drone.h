/**
 * Drone Header
 * */
#ifndef DRONE_H
#define DRONE_H

#include <time.h>
#include <thread>
#include "Accelerometer.h"
#include "Motor.h"
#include "Server.h"

class Drone
{
public:
    Drone();
    Drone(int port, int freq, int m1_pin, int m2_pin, int m3_pin, int m4_pin, int acc_da_pin, int acc_clk_pin, int cc);
    ~Drone();

    int handleInstruction(char INS);
    void *checkAlt(); //called by server, polls accelerometer to maintain drones altitude if state is ROT_L/R, RESET, ACC_X/Y
    signed short *getGyroData();
    signed short *getAccData();
    int setAllMotors(int speed);
    int setMotorSpeed(int motor_id, int speed); //sets all motors to the same speed
    void startDrone();
    
private:
    Motor *motors[4];
    Accelerometer *acc;
    Server *server;
    signed short *acc_data;
    signed short *gyro_data;
    std::thread *hover;
    int calibrated;
    int getAvgMotorSpeed();
};
#endif