/**
 * Motor Header
 * */
#ifndef MOTOR_H
#define MOTOR_H


class Motor{
public:
    Motor(int pin,int frequency);
    ~Motor();
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
    bool is_on;
    int duty;
    int frequency;
    unsigned int pin;
};
#endif