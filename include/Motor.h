/**
 * Motor Header
 * */
#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>
#include "Configuration.h"

class Motor{
    private:
        int speed;
        int range;
        int frequency;
        unsigned int pin;

    public:
        Motor(int pin);
        Motor(int pin,int frequency);
        Motor(int pin, int frequency, int pwm_range);
        ~Motor();

        /** Getters **/
        int getPin() {
        }
        int getFrequency() {
        }
        int getDutyRange() {
        }
        int getSpeed() {
        }

        /** Setters **/
        void setFrequency(int frequency);
        void setDutyRange(int range);
        int setSpeed(int speed);

        int stop();
        bool isRunning() {
            return (this -> speed != 0);
        }
};
#endif