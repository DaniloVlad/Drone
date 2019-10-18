/**
 * Drone Header
 * */
#ifndef DRONE_H
#define DRONE_H
#include "Accelerometer.h"
#include "Motor.h"

class Drone{
public:
    Drone(int pin,int frequency);
    ~Drone();
    bool isRunning();
    int start(int speed);
    int stop();

    /** Getters **/
    int getPin();
    int getFrequency();
    int getDutyRange();

    /** Setters **/
    void setFrequency(int frequency);
    void setDutyRange();
private:
    int Motors[4];
    
};
#endif