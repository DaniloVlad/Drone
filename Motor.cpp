#include "include/Motor.h"

/* Function: Motor Constructor
*   @breif - Constructs and sets the base motor with only the pin parameter. 
*   @param pin - the pin address for the device motor.
*/  
Motor::Motor(int pin) {
  
    this -> pin = pin;
    // If the GPIO mode is unable to be set to alternative mode 0, send an error message and exit.
    if(gpioSetMode(pin, PI_OUTPUT) != 0) {
        perror("Couldn't initialize pin as PI_OUTPUT");
        exit(EXIT_FAILURE);
    }

    this -> setDutyRange(MOTOR_PWM_RANGE);
    this -> setFrequency(MOTOR_PWM_FREQUENCY);
}

/* Function: Motor Constructor
*   @breif - Constructs and sets the motor with only the pin and frequency paramaters.
*   @param pin - the pin address for the device motor.
*   @param frequency - the frequency that the signal will be transmitted on.
*/  
Motor::Motor(int pin, int frequency) {
  
    this -> pin = pin;  
    // If the GPIO mode is unable to be set to alternative mode 0, send an error message and exit.
    if(gpioSetMode(pin, PI_OUTPUT) != 0) {
        perror("Couldn't initialize pin as PI_OUTPUT");
        exit(EXIT_FAILURE);
    }

    this -> setDutyRange(MOTOR_PWM_RANGE);
    this -> setFrequency(frequency);
}

/* Function: Motor Constructor
*   @breif - Constructs and sets the motor with only the pin and frequency paramaters.
*   @param pin - the pin address for the device motor.
*   @param frequency - the frequency that the signal will be transmitted on.
*   @param pwm_range - the range of the signal.
*/  
Motor::Motor(int pin, int frequency, int pwm_range) {
  
    this -> pin = pin;
    // If the GPIO mode is unable to be set to alternative mode 0, send an error message and exit.
    if(gpioSetMode(pin, PI_OUTPUT) != 0) {
        perror("Couldn't initialize pin as PI_OUTPUT");
        exit(EXIT_FAILURE);
    }

    this -> setDutyRange(pwm_range);
    this -> setFrequency(frequency);
}

/* Function: Motor Deconstructor
*   @breif - Deconstructor method for the motor.
*/  
Motor::~Motor() {
    this -> stop();
}

/* Function: setFrequency
*   @breif - Setter function for the motor frequency, and sends an error message and exit's if the setter fails.
*   @param frequency - the frequency that the signal will be transmitted on.
*/  
void Motor::setFrequency(int frequency) {
    if((this -> frequency = gpioSetPWMfrequency(this -> pin, frequency)) == PI_BAD_USER_GPIO) {
        perror("Couldn't set PWM frequency");
        exit(EXIT_FAILURE);
    }
}

/* Function: setDutyRange
*   @breif - Setter function for the motor range, and sends an error message and exit's if the setter fails.
*   @param range - the range of the signal.
*/  
void Motor::setDutyRange(int range) {
    if((this -> range = gpioSetPWMrange(this -> pin, range)) == (PI_BAD_USER_GPIO | PI_BAD_DUTYRANGE)) {
        perror("Couldn't set PWM range");
        exit(EXIT_FAILURE);
    }
}

/* Function: setSpeed
*   @breif - Setter function for the motor speed.
* 	On error returns -1, otherwise returns the speed.
*   @param speed - speed the motor's are spinning.
*/  
int Motor::setSpeed(int speed) {
  	// If the speed is out of range, it returns -1.
    if(speed > this -> range || speed < 0) {
        perror("Speed out of range");
        return -1;
    }
  	// Attempt to return the speed.
    else {
      	// First check if the GPIO PWM can be set to 0 (off), send an error message and exit.
        if(gpioPWM(this -> pin, speed) != 0) {
            perror("Error setting speed");
            exit(EXIT_FAILURE);
        }
      	// If everything works as intended, the speed is returned
        return (this -> speed = speed);
    }
}

/* Function: stop
*   @breif - Stop condition function for the motor, which returns 0 when successful, and -1 on an error.
*/  
int Motor::stop() {
    return (this -> setSpeed(0));
}
