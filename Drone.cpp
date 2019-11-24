#include "Drone.h"
#include "Motor.h"
#include "STATE.h"
#include "Server.h"

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
	//do through a loop
	delete this -> motors[0];
}

int Drone::handleInstruction(char INS){


}


void Drone::checkAlt(){



}

int Drone::*getGyroData(){


}

int Drone::*getAccData(){

}

int Drone::rotateLeft(){



}

int Drone::rotateRight(){


}

int Drone::moveForward(){


}

int Drone::moveBackward(){


}

int Drone::moveLeft(){


}


int Drone::moveRight(){


}

int Drone::climb(){


}

int Drone::lower(){


}


int Drone::setAllMotors(int speed){


}


int Drone::setMotorSpeed(int motor_id, int speed){


}


;
