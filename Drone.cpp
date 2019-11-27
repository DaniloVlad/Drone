#include "include/Drone.h"

Drone::Drone() {
    this -> motors[0] = new Motor(GPIO_MOTOR_1);
    this -> motors[1] = new Motor(GPIO_MOTOR_2);
    this -> motors[2] = new Motor(GPIO_MOTOR_3);
    this -> motors[3] = new Motor(GPIO_MOTOR_4);
    this -> acc = new Accelerometer();
    this -> server = new Server();

    this -> acc_data[0] = {0};
    this -> acc_data[1] = {0};
    this -> acc_data[2] = {0};


}
Drone::Drone(int port, int freq, int m1_pin, int m2_pin, int m3_pin, int m4_pin, int acc_da_pin, int acc_clk_pin, int cc) {
	
	this -> motors[0] = new Motor(m1_pin);
	this -> motors[1] = new Motor(m2_pin);
	this -> motors[2] = new Motor(m3_pin);
	this -> motors[3] = new Motor(m4_pin);
	this -> acc = new Accelerometer();
	//initialize with a loop
	this -> acc_data[0] = 0;
	this -> acc_data[1] = 0;
	this -> acc_data[2] = 0;
}

Drone::~Drone(){
	for(int i = 0; i < 4; i++)
	    delete this -> motors[i];

    delete this -> acc;
    delete this -> server;
}

int Drone::handleInstruction(char INS){
    printf("Got character %c\n", INS);
    switch (INS)
    {
    case 'c':
        //calibrate motors
        break;
    case 'a':
        //rotate left
        break;
    case 'd':
        //rotate right
        break;
    case 'w':
        //raise
        break;
    case 's':
        //lower
        break;
    case '<':
        //move left
        break;
    case '>':
        //move right
        break;
    case '^':
        //move forward
        break;
    case 'b':
        //move backward
        break;

    default:
        break;
    }
    return 0;

}


void Drone::checkAlt(){



}

int Drone::*getGyroData(){
return 0;

}

int Drone::*getAccData(){
return 0;
}

int Drone::rotateLeft(){

return 0;

}

int Drone::rotateRight(){
return 0;

}

int Drone::moveForward(){
return 0;

}

int Drone::moveBackward(){

return 0;
}

int Drone::moveLeft(){

return 0;
}


int Drone::moveRight(){

return 0;
}

int Drone::climb(){

return 0;
}

int Drone::lower(){
return 0;

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
        printf("Read data...\n");
        printf("%s\n", buff);
    }
    printf("Exiting...\n");
}