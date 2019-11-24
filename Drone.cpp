#include "Drone.h"
#include "Motor.h"
#include "STATE.h"
#include "Server.h"

Drone::Drone(int port, int freq, int m1_pin, int m2_pin, int m3_pin, int m4_pin, int acc_da_pin, int acc_clk_pin, int cc){
	


}

Drone::~Drone(){
    for(int i = 0; i < 4; i++)
        delete motors[i];
    delete acc;
    delete server;

}

int Drone::handleInstruction(char INS){


}


STATE Drone::getState(){


}

void Drone::checkAlt(){



}

int *Drone::getGyroData(){


}

int *Drone::getAccData(){

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

void Drone::startDrone() {

}

