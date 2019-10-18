/**
 * Accelerometer Header
 * */
#ifndef Accelerometer_H
#define Accelerometer_H


class Accelerometer{
public:
    Accelerometer(int data_pin,int clock_pin,int clock_cycle);
    ~Accelerometer();

    /** Getters **/
    int getGyroX();
    int getGyroY();
    int getGyroZ();
    int getAccX();
    int getAccY();
    int getAccZ();
    int* getGyroXYZ();
    int* getAccXYZ();

    /** Setters **/
    //nothing right now
private:
    int data_pin;
    int clock_pin;
    int clock_cycle;
};
#endif