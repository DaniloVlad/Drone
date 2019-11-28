#include "include/Drone.h"


// motors[0] --- motors[1]
//   |            |
// motors[2] --- motors[3]

// 0 & 3 => Clockwise
// 1 & 2 => Counter Clockwise

Drone::Drone() {
    this -> motors[0] = new Motor(GPIO_MOTOR_1);
    this -> motors[1] = new Motor(GPIO_MOTOR_2);
    this -> motors[2] = new Motor(GPIO_MOTOR_3);
    this -> motors[3] = new Motor(GPIO_MOTOR_4);
    this -> acc = new Accelerometer();
    this -> server = new Server();

    //set thread id to 0 (ie not running)
    this -> hover = 0;

    this -> acc_data = new signed short(3);
    this -> gyro_data = new signed short(3);
    for(int i = 0; i < 3; i++) {
        this -> acc_data[i] = 0;
        this -> gyro_data[i] = 0;
    }
    this -> calibrated = 0;

}
Drone::Drone(int port, int freq, int m1_pin, int m2_pin, int m3_pin, int m4_pin, int acc_da_pin, int acc_clk_pin, int cc) {
	
	this -> motors[0] = new Motor(m1_pin);
	this -> motors[1] = new Motor(m2_pin);
	this -> motors[2] = new Motor(m3_pin);
	this -> motors[3] = new Motor(m4_pin);
	this -> acc = new Accelerometer();
    this -> server = new Server(port);
    
    //set thread id to 0 (ie not running)
    this -> hover = 0;

    this -> acc_data = new signed short(3);
    this -> gyro_data = new signed short(3);
	//initialize with a loop
    for(int i = 0; i < 3; i++) {
        this -> acc_data[i] = 0;
        this -> gyro_data[i] = 0;
    }
    this -> calibrated = 0;
}

Drone::~Drone(){
	for(int i = 0; i < 4; i++)
	    delete this -> motors[i];

    delete this -> acc;
    delete this -> server;
    delete this -> acc_data;
    delete this -> gyro_data;
}

int Drone::handleInstruction(char INS){
    printf("Got character %c\n", INS);
    switch (INS)
    {
    case 'c':
        if(this -> calibrated == 0) {
            this -> setAllMotors(2000);
            this -> calibrated = 1;
        }
        else {
            this -> setAllMotors(1000);
        }
        break;

    case 'a':
        //rotate left
        int clockwise = this -> motors[0] -> getSpeed();
        int counterClockWise = this -> motors[1] -> getSpeed();

        //prevent the drone from flipping over
        if(counterClockWise - clockwise >= 400)
            break;

        //prevent drone from going over max or under min!
        if(clockwise - 100 < 1100) clockwise = 1200;
        if(counterClockWise + 100 > 2000) counterClockWise = 1900;

        if(counterClockWise >= clockwise + 200) {
            this -> setMotorSpeed(0, clockwise - 100);
            this -> setMotorSpeed(3, clockwise - 100);
            this -> setMotorSpeed(1, counterClockWise + 100);
            this -> setMotorSpeed(2, counterClockWise + 100);
        }
        else {
            //speeds are within 100 steps (10%) of eachother
            int diff = 0;
            if(clockwise > counterClockWise) 
                diff = clockwise - counterClockWise;
            
            this -> setMotorSpeed(0, clockwise - 100);
            this -> setMotorSpeed(3, clockwise - 100);
            this -> setMotorSpeed(1, counterClockWise + diff + 100);
            this -> setMotorSpeed(2, counterClockWise + diff + 100);
        }
        break;

    case 'd':
        //rotate right
        int clockwise = this -> motors[0] -> getSpeed();
        int counterClockWise = this -> motors[1] -> getSpeed();

        //prevent the drone from flipping over
        if(clockwise - counterClockWise >= 400) 
            break;

        //prevent drone from going over max or under min!
        if(counterClockWise - 100 < 1100) counterClockWise = 1200;
        if(clockwise + 100 > 2000) clockwise = 1900;
        
        if(clockwise >= counterClockWise + 200) {
            this -> setMotorSpeed(0, clockwise + 100);
            this -> setMotorSpeed(3, clockwise + 100);
            this -> setMotorSpeed(1, counterClockWise - 100);
            this -> setMotorSpeed(2, counterClockWise - 100);
        }
        else {
            //speeds are within 100 steps (10%) of eachother
            int diff = 0;
            if(counterClockWise > clockwise) 
                diff = counterClockWise - clockwise;
            
            this -> setMotorSpeed(0, clockwise + diff + 100);
            this -> setMotorSpeed(3, clockwise + diff + 100);
            this -> setMotorSpeed(1, counterClockWise - 100);
            this -> setMotorSpeed(2, counterClockWise - 100);
        }
        break;

    case 'w':
        //raise, increase all motor speeds
        int avgSpeed = this -> getAvgMotorSpeed();

        //prevent drone from going above maximum
        if(avgSpeed + 100 > 2000) avgSpeed = 1900;

        this -> setAllMotors(avgSpeed + 100);
        break;

    case 's':
        //lower, decrease all motor speeds
        int avgSpeed = this -> getAvgMotorSpeed();

        //prevent drone from going beyond minimum
        if(avgSpeed - 100 < 1100) avgSpeed = 1200;

        this -> setAllMotors(avgSpeed - 100);
        break;

    case '<':
        //move left, Motors 1 & 3 should be spin faster than 0 & 2
        int leftBank = this -> motors[0] -> getSpeed();
        int rightBank = this -> motors[1] -> getSpeed();

        //prevent the drone from flipping over
        if(rightBank - leftBank >= 400)
            break;

        //prevent motors from exceeding max/min
        if(leftBank - 100 < 1100) leftBank = 1200;
        if(rightBank + 100 > 2000) rightBank = 1900;

        if(rightBank >= leftBank + 200) {
            this -> setMotorSpeed(0, leftBank - 100);
            this -> setMotorSpeed(2, leftBank - 100);

            this -> setMotorSpeed(3, rightBank + 100);
            this -> setMotorSpeed(1, rightBank + 100);
        
        }
        else {
            int diff = 0;
            if(leftBank > rightBank)
                diff = leftBank - rightBank;
            
            this -> setMotorSpeed(0, leftBank - 100);
            this -> setMotorSpeed(2, leftBank - 100);

            this -> setMotorSpeed(3, rightBank + diff + 100);
            this -> setMotorSpeed(1, rightBank + diff + 100);
        }
        break;

    case '>':
        //move right, Motors 0 & 2 > Motors 1 & 3
        int leftBank = this -> motors[0] -> getSpeed();
        int rightBank = this -> motors[1] -> getSpeed();

        //prevent the drone from flipping over
        if(leftBank - rightBank >= 400)
            break;

        //prevent motors from exceeding max/min
        if(leftBank + 100 > 2000) leftBank = 1900;
        if(rightBank - 100 < 1100) rightBank = 1200;

        if(leftBank >= rightBank + 200) {
            this -> setMotorSpeed(0, leftBank + 100);
            this -> setMotorSpeed(2, leftBank + 100);

            this -> setMotorSpeed(3, rightBank - 100);
            this -> setMotorSpeed(1, rightBank - 100);
        
        }
        else {
            int diff = 0;
            if(rightBank > leftBank)
                diff = rightBank - leftBank;
            
            this -> setMotorSpeed(0, leftBank + diff + 100);
            this -> setMotorSpeed(2, leftBank + diff + 100);

            this -> setMotorSpeed(3, rightBank - 100);
            this -> setMotorSpeed(1, rightBank - 100);
        }
        break;

    case '^':
        //move forward, Motors 0 & 1 > Motors 2 & 3
        int frontBank = this -> motors[0] -> getSpeed();
        int backBank = this -> motors[2] -> getSpeed();

        //prevent the drone from flipping over
        if(backBank - frontBank >= 400)
            break;
        
        //prevent motors from exceeding max/min
        if(frontBank - 100 < 1100) frontBank = 1200;
        if(backBank + 100 > 2000) backBank = 1900;

        if(backBank >= frontBank + 200) {
            this -> setMotorSpeed(0, frontBank - 100);
            this -> setMotorSpeed(1, frontBank - 100);

            this -> setMotorSpeed(2, backBank + 100);
            this -> setMotorSpeed(3, backBank + 100);
        
        }
        else {
            int diff = 0;
            if(frontBank > backBank)
                diff = frontBank - backBank;
            
            this -> setMotorSpeed(0, frontBank - 100);
            this -> setMotorSpeed(1, frontBank - 100);

            this -> setMotorSpeed(3, backBank + diff + 100);
            this -> setMotorSpeed(1, backBank + diff + 100);
        }
        break;

    case 'b':
        //move backward, Motors 2 & 3 > Motors 0 & 1
        int frontBank = this -> motors[0] -> getSpeed();
        int backBank = this -> motors[2] -> getSpeed();

        
        //prevent the drone from flipping over
        if(frontBank - backBank >= 400) 
            break; 

        //prevent motors from exceeding max/min
        if(frontBank + 100 > 2000) frontBank = 1900;
        if(backBank - 100 < 1100) backBank = 1200;
        if(frontBank >= backBank + 200) {
            this -> setMotorSpeed(0, frontBank + 100);
            this -> setMotorSpeed(1, frontBank + 100);

            this -> setMotorSpeed(2, backBank - 100);
            this -> setMotorSpeed(3, backBank - 100);
        
        }
        else {
            int diff = 0;
            if(backBank > frontBank)
                diff = backBank - frontBank;
            
            this -> setMotorSpeed(0, frontBank + diff + 100);
            this -> setMotorSpeed(1, frontBank + diff + 100);

            this -> setMotorSpeed(3, backBank - 100);
            this -> setMotorSpeed(1, backBank - 100);
        }
        break;

    case 'x':
        //turn off all motors
        this -> setAllMotors(0);
        break;
    case 'l':
        //set motors to minimum speed
        this -> setAllMotors(1100);
        break;

    case 'h':
        //Expiremental hover!
        if(this -> hover == 0) {
            if(pthread_create(&this -> hover, NULL, (this -> checkAlt), NULL) == 0) {
                printf("Creating hover thread!");
            }
        }
        else
            this -> hover = 0;
        
        
        break;
    default:
        break;
    }
    return 0;

}


void Drone::checkAlt(){
    //on default mode accelerometer samples at 20hz ie 20 updates to values per second
    while(this -> hover != 0) {
        float z = (float) this -> acc -> getAccZ();
        int speed = this -> getAvgMotorSpeed();
        if(z < 0) z = (-1) * z;

        if(z > 1) {
            if(speed - 25 < 1000) speed = 1025;
            this -> setAllMotors(speed - 25);
        }
        else {
            if(speed + 25 > 2000) speed = 1975;
            this -> setAllMotors(speed + 25);
        }
        timespec time;
        time.tv_nsec = 1000000000/10;
        nanosleep(&time, NULL);
    }
    return;

}

signed short *Drone::getGyroData() {
    this -> gyro_data = this -> acc -> getGyroXYZ();
    return this -> gyro_data;
}

signed short *Drone::getAccData(){
    this -> acc_data = this -> acc -> getAccXYZ();
    return this -> acc_data;
}

int Drone::setAllMotors(int speed){
    if(motors[0] -> setSpeed(speed) < 0 || motors[1] -> setSpeed(speed) < 0 || motors[2] -> setSpeed(speed) < 0 || motors[3] -> setSpeed(speed) < 0) {
        return -1;
    }
    return 0;
}


int Drone::setMotorSpeed(int motor_id, int speed){
    if(motors[motor_id] -> setSpeed(speed) < 0) {
        return -1;
    }
    return 0;
}

void Drone::startDrone() {
    char buff[256];
    this -> server -> startServer();
    printf("Drone attempting read...\n");
    while(this -> server -> receive(&buff, 1) > 0) {
        handleInstruction(buff[0]);
    }
    printf("Exiting...\n");
}

int Drone::getAvgMotorSpeed() {
    return ((this -> motors[0] -> getSpeed() + this -> motors[1] -> getSpeed() + this -> motors[2] -> getSpeed() + this -> motors[3] -> getSpeed()) / 4);
}