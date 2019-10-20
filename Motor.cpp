#include "include/Motor.h"

Motor::Motor(int pin) {
    this -> pin = pin;

    if(gpioSetMode(pin, PI_OUTPUT) != 0) {
        perror("Couldn't initialize pin as PI_OUTPUT");
        exit(EXIT_FAILURE);
    }

    this -> setDutyRange(MOTOR_PWM_RANGE);
    this -> setFrequency(MOTOR_PWM_FREQUENCY);

}

Motor::Motor(int pin, int frequency) {
    this -> pin = pin;  

    if(gpioSetMode(pin, PI_OUTPUT) != 0) {
        perror("Couldn't initialize pin as PI_OUTPUT");
        exit(EXIT_FAILURE);
    }

    this -> setDutyRange(MOTOR_PWM_RANGE);
    this -> setFrequency(frequency);
}

Motor::Motor(int pin, int frequency, int pwm_range) {
    this -> pin = pin;

    if(gpioSetMode(pin, PI_OUTPUT) != 0) {
        perror("Couldn't initialize pin as PI_OUTPUT");
        exit(EXIT_FAILURE);
    }

    this -> setDutyRange(pwm_range);

    this -> setFrequency(frequency);
}

Motor::~Motor() {
    this -> stop();
}

void Motor::setFrequency(int frequency) {
    if((this -> frequency = gpioSetPWMfrequency(this -> pin, frequency)) == PI_BAD_USER_GPIO) {
        perror("Couldn't set PWM frequency");
        exit(EXIT_FAILURE);
    }
}

void Motor::setDutyRange(int range) {
    if((this -> range = gpioSetPWMrange(this -> pin, range)) == (PI_BAD_USER_GPIO | PI_BAD_DUTYRANGE)) {
        perror("Couldn't set PWM range");
        exit(EXIT_FAILURE);
    }
}

//-1 on error, otherwise returns speed
int Motor::setSpeed(int speed) {
    if(speed > this -> range || speed < 0) {
        perror("Speed out of range");
        return -1;
    }
    else {
        if(gpioPWM(this -> pin, speed) != 0) {
            perror("Error setting speed");
            exit(EXIT_FAILURE);
        }
        return (this -> speed = speed);
    }
}

//-1 on error, 0 on success
int Motor::stop() {
    return (this -> setSpeed(0));
}