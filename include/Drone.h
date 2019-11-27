/**
 * Drone Header
 * */
#ifndef DRONE_H
#define DRONE_H

#include "Accelerometer.h"
#include "Motor.h"
#include "STATE.h"
#include "Server.h"
class Drone
{
public:
    Drone();
    Drone(int port, int freq, int m1_pin, int m2_pin, int m3_pin, int m4_pin, int acc_da_pin, int acc_clk_pin, int cc);
    ~Drone();
    int handleInstruction(char INS);
    //called by server after receiving instruction
    STATE *getState() {
        return this -> current_state;
    }
    void checkAlt(); //called by server, polls accelerometer to maintain drones altitude if state is ROT_L/R, RESET, ACC_X/Y
    int *getGyroData();
    int *getAccData();
    int rotateLeft();
    int rotateRight();
    int moveForward();
    int moveBackward();
    int moveLeft();
    int moveRight();
    int climb();
    int lower();
    int setAllMotors(int speed);
    int setMotorSpeed(int motor_id, int speed); //sets all motors to the same speed
    int land();
    void startDrone();
    
private:
    Motor *motors[4];
    Accelerometer *acc;
    STATE *current_state;
    Server *server;
    int acc_data[3];
    int gyro_data[3];
};
#endif