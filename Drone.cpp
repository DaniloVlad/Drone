#include "include/Drone.h"


// motors[0] --- motors[1]
//   |            |
// motors[2] --- motors[3]

// 0 & 3 => Clockwise
// 1 & 2 => Counter Clockwise


/**
*Drone Constructor
*@brief  - constructs a drone object that will prepare  all the components required by the physical drone. including the mototrs, the accelerometer, the server,the acc_data, gyro_data, calibrated, and hover 
*/
Drone::Drone() {
    // the creation of motor objects to control each motor
    this -> motors[0] = new Motor(GPIO_MOTOR_1);
    this -> motors[1] = new Motor(GPIO_MOTOR_2);
    this -> motors[2] = new Motor(GPIO_MOTOR_3);
    this -> motors[3] = new Motor(GPIO_MOTOR_4);
    // the creation of accelerometer object
    this -> acc = new Accelerometer();
    // the initializaiton of server class to communicate with drone
    this -> server = new Server();

    //set thread id to 0 (ie not running) and initialize all values to 0 using a for loop
    this -> hover = NULL;
    this -> acc_data = new signed short(3);
    this -> gyro_data = new signed short(3);
    for(int i = 0; i < 3; i++) {
        this -> acc_data[i] = 0;
        this -> gyro_data[i] = 0;
    }
    this -> calibrated = 0;
}

/**
*Enhanced Drone Constructor
*@brief - constructs a drone object but using the parameters to initialize with motor connections, the specific port, and so forth 
*@param port - this holds the port number of which the server will send information back to
*@param m1_pin - this holds the pin for motor 0
*@param m2_pin - this holds the pin for motor 1
*@param m3_pin - this holds the pin for motor 2
*@param m4_pin - this holds the pin for motor 3
*/
Drone::Drone(int port, int freq, int m1_pin, int m2_pin, int m3_pin, int m4_pin, int acc_da_pin, int acc_clk_pin, int cc) {
	// equivalent comments as in Drone::Drone() but with each using a parameter value rather than a reference value
	this -> motors[0] = new Motor(m1_pin);
	this -> motors[1] = new Motor(m2_pin);
	this -> motors[2] = new Motor(m3_pin);
	this -> motors[3] = new Motor(m4_pin);
	this -> acc = new Accelerometer();
    this -> server = new Server(port);
    
    //set thread id to 0 (ie not running)
    this -> hover = NULL;

    this -> acc_data = new signed short(3);
    this -> gyro_data = new signed short(3);
	//initialize with a loop
    for(int i = 0; i < 3; i++) {
        this -> acc_data[i] = 0;
        this -> gyro_data[i] = 0;
    }
    this -> calibrated = 0;
}

/**
*Destructor function of Drone
*@brief - this is the destructor function for Drone, it frees any memory allocated
*/
Drone::~Drone(){
   // delete all motors in the motors array
    for(int i = 0; i < 4; i++)
    	delete this -> motors[i];
    // delete the rest of the objects in the Drone class that are referenced
    delete this -> acc;
    delete this -> server;
    delete this -> acc_data;
    delete this -> gyro_data;
}

/**
    Function to handle instructions
    @brief - this handleInstruction function essentially handles the instruction it recieved from the server class to apply the appropriate command
    @param  INS - this holds the instruction character which will correspond to a task to be performed by the drone
*/
int Drone::handleInstruction(char INS){
    printf("Got character %c\n", INS);
    switch (INS)
    {

   // this is the case were we are trying to calibrate the drone
    case 'c':
        {
	    // when not initialized we set all the motors to a value of 2000 then set calibrated value to 1
            if(this -> calibrated == 0) {
                this -> setAllMotors(2000);
                this -> calibrated = 1;
            }
           // if drone as been previously calibrated, we set the value of all motors to 1000
            else {
                this -> setAllMotors(1000);
            }
            break;
        }
        
    // this is the case for having the drone move to the left
    case 'a':
        {
            //rotate left by adjusting clockwise and counterclockwise values to corresponding motor speeds to have the motor
	    // in the correct direction
            int clockwise = this -> motors[0] -> getSpeed();
            int counterClockWise = this -> motors[1] -> getSpeed();

            // check to prevent the drone from flipping over
            if(counterClockWise - clockwise >= 400)
                break;

            //check to prevent drone from going over max or under min!
	    // case for clockwise speed is less than 1100
            if(clockwise - 100 < 1100) clockwise = 1200;
	    // case for counterClockWise speed being too high
            if(counterClockWise + 100 > 2000) counterClockWise = 1900;
	    // case for 
            if(counterClockWise >= clockwise + 200) {
                this -> setMotorSpeed(0, clockwise - 100);
                this -> setMotorSpeed(3, clockwise - 100);
                this -> setMotorSpeed(1, counterClockWise + 100);
                this -> setMotorSpeed(2, counterClockWise + 100);
            }
            else {
                //speeds are within 100 steps (10%) of each other
                int diff = 0;
                if(clockwise > counterClockWise) 
                    diff = clockwise - counterClockWise;
                
                this -> setMotorSpeed(0, clockwise - 100);
                this -> setMotorSpeed(3, clockwise - 100);
                this -> setMotorSpeed(1, counterClockWise + diff + 100);
                this -> setMotorSpeed(2, counterClockWise + diff + 100);
            }
            break;

        }

    case 'd':
        {

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
        }

    case 'w':
        {
            //raise, increase all motor speeds
            int avgSpeed = this -> getAvgMotorSpeed();

            //prevent drone from going above maximum
            if(avgSpeed + 100 > 2000) avgSpeed = 1900;

            this -> setAllMotors(avgSpeed + 100);
            break;
        }

    case 's':
        {
            //lower, decrease all motor speeds
            int avgSpeed = this -> getAvgMotorSpeed();

            //prevent drone from going beyond minimum
            if(avgSpeed - 100 < 1100) avgSpeed = 1200;

            this -> setAllMotors(avgSpeed - 100);
            break;
        }

    case '<':
        {
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
        }

    case '>':
        {

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
        }

    case '^':
        {

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
        }

    case 'b':
        {

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
        }

    case 'x':
        {
            //turn off all motors
            this -> setAllMotors(0);
            break;
        }
    case 'l':
        {
            //set motors to minimum speed
            this -> setAllMotors(1100);
            break;
        }

    case 'h':
        {
            //Expiremental hover!
            if(this -> hover != NULL) {
                this -> hover = new std::thread(&Drone::checkAlt, this);
            }
            else
                this -> hover = NULL;
            
            
            break;
        }
    default:
        break;
    }

    return 0;

}


void *Drone::checkAlt(){
    //on default mode accelerometer samples at 20hz ie 20 updates to values per second
    while(this -> hover != NULL) {
        float z = (float) this -> acc -> getAccZ();
        int speed = this -> getAvgMotorSpeed();
        if(z < 0) z = (-1) * z;

        if(z > 1) {
            if(speed - 25 < 1100) speed = 1125;
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
    return NULL;
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
