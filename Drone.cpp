/**
 * @class Drone 
 * @brief Drone object that will handle all socket, motor, and accelerometer utilities
 * */

#include "include/Drone.h"

// these comments are for reference of where each motor is
// with the left side being with motors 0 and 2 and the right side
// with motors 1 and 3
// this will allow you to understand why each motor's speeds are changed
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
*@param freq - 
*@param cc, acc_clk_pin, acc_da_pin - are required for Accelerometer object which were not implemented but could be in the near future due to complexity
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
    // if  the average motors are not initalized to minimum speed then we adjust them to that speed
    if(this -> getAvgMotorSpeed() == 0) {
        printf("All motors are 0, starting at minimum!\n");
        this -> setAllMotors(1100);
    }
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
        
    // this is the case for having the drone rotate to the the left
    case 'a':
        {
            //rotate left by adjusting clockwise and counterclockwise values to corresponding motor steps to have the motor
	    // move in the correct direction
            int clockwise = this -> motors[0] -> getSpeed();
            int counterClockWise = this -> motors[1] -> getSpeed();

            // check to prevent the drone from flipping over
            if(counterClockWise - clockwise >= 400)
                break;

            //check to prevent drone from going over max or under min!
	    // case for clockwise steps is less than 1100, hence the speeds too low
            if(clockwise - 100 < 1100) clockwise = 1200;
	    // case for counterClockWise steps being too high = speeds being too high
            if(counterClockWise + 100 > 2000) counterClockWise = 1900;
	    // case for when the counterClockWise steps is greater than or equal to clockwise steps + 200
	    // which equates to the counterClockWise speed for  the drone motors being too fast 
            if(counterClockWise >= clockwise + 200) {
                this -> setMotorSpeed(0, clockwise - 100);
                this -> setMotorSpeed(3, clockwise - 100);
                this -> setMotorSpeed(1, counterClockWise + 100);
                this -> setMotorSpeed(2, counterClockWise + 100);
            }
   	    // when the speeds are very close to one another
            else {
                //speeds are within 100 steps (10%) of each other
                int diff = 0;
		// when the steps of left motor are greather than right motor
                if(clockwise > counterClockWise) 
                    diff = clockwise - counterClockWise;
                // set the motor speeds accordingly to ensure that the motor moves to the left
		// by reducing the speed of the motors to the left and increasing the speed on the motors
		// on the right of the motor
                this -> setMotorSpeed(0, clockwise - 100);
                this -> setMotorSpeed(3, clockwise - 100);
                this -> setMotorSpeed(1, counterClockWise + diff + 100);
                this -> setMotorSpeed(2, counterClockWise + diff + 100);
            }
            break;

        }
    // this is the case to rotate the drone to the right 
    case 'd':
        {
	    // to rotate the drone to the right we set stepping of 
	    // motor 0 to clockwise and motor 1's stepping to counterclockwise value
            int clockwise = this -> motors[0] -> getSpeed();
            int counterClockWise = this -> motors[1] -> getSpeed();

            //check to make sure the drone does not flipping over if the difference is  significant
            if(clockwise - counterClockWise >= 400) 
                break;

            //check to prevent drone from going over max or under min!
	    // this checks to make sure counterclockwise speed is not too slow
            if(counterClockWise - 100 < 1100) counterClockWise = 1200;
	    // checks to make sure clockwise speed is not too high
            if(clockwise + 100 > 2000) clockwise = 1900;
            
            // now we adjust the speeds of motors if the clockwise speed is greater than counterClockWise speed
            if(clockwise >= counterClockWise + 200) {
                this -> setMotorSpeed(0, clockwise + 100);
                this -> setMotorSpeed(3, clockwise + 100);
                this -> setMotorSpeed(1, counterClockWise - 100);
                this -> setMotorSpeed(2, counterClockWise - 100);
            }
	    // we change the motor speeds differently if the speeds are very close
            else {
                //speeds are within 100 steps (10%) of eachother
                int diff = 0;
		// we calculate the differnce if counterClockWise directionis greater than clockwise
                if(counterClockWise > clockwise) 
                    diff = counterClockWise - clockwise;
                // and we utilize this differnce in adjusting the speeds of the motors
                this -> setMotorSpeed(0, clockwise + diff + 100);
                this -> setMotorSpeed(3, clockwise + diff + 100);
                this -> setMotorSpeed(1, counterClockWise - 100);
                this -> setMotorSpeed(2, counterClockWise - 100);
            }
            break;
        }
    // case for when we want to raise  the altitude of the drone
    case 'w':
        {
            //raise, increase all motor speeds
            int avgSpeed = this -> getAvgMotorSpeed();

            //prevent drone from going above maximum
            if(avgSpeed + 100 > 2000) avgSpeed = 1900;

	    // set the motor speeds to the average speed + 10 stepping
            this -> setAllMotors(avgSpeed + 100);
            break;
        }
    // case for when we want to decrease the altitude of the drone
    case 's':
        {
            //lower, decrease all motor speeds
            int avgSpeed = this -> getAvgMotorSpeed();

            //prevent drone from going beyond minimum
            if(avgSpeed - 100 < 1100) avgSpeed = 1200;
	    
	    // set all motor speeds to 100 steps less than average speed
            this -> setAllMotors(avgSpeed - 100);
            break;
        }
    // case for drone to move in the left direction
    case '<':
        {
            //move left, Motors 1 & 3 should be spin faster than 0 & 2
            int leftBank = this -> motors[0] -> getSpeed();
            int rightBank = this -> motors[1] -> getSpeed();

            //prevent the drone from flipping over
            if(rightBank - leftBank >= 400)
                break;

            //prevent motors from exceeding max/min

	    // if the left bank is too slow we re-adjust the stepping
            if(leftBank - 100 < 1100) leftBank = 1200;
	    // if the right bank is too fast then we lower the stepping
            if(rightBank + 100 > 2000) rightBank = 1900;
	    // if the right bank is significatly larger than left bank then
	    // the way we change the motor speeds will be depending upon
	    // just pure subtracting and adding 100 steppings
            if(rightBank >= leftBank + 200) {
                this -> setMotorSpeed(0, leftBank - 100);
                this -> setMotorSpeed(2, leftBank - 100);

                this -> setMotorSpeed(3, rightBank + 100);
                this -> setMotorSpeed(1, rightBank + 100);
            }
	    // the else case is when you end up with a stepping case
	    // with the right and left bank being within 10% (100 steppings
            else {
                int diff = 0;
		// calculate the difference if leftbank is greater than right bank
                if(leftBank > rightBank)
                    diff = leftBank - rightBank;
                // use this difference to adjust the motor speed of left and right bank
                this -> setMotorSpeed(0, leftBank - 100);
                this -> setMotorSpeed(2, leftBank - 100);

                this -> setMotorSpeed(3, rightBank + diff + 100);
                this -> setMotorSpeed(1, rightBank + diff + 100);
            }
            break;
        }
    // this is the case for moving the drone in the right direction
    case '>':
        {
            //move right, Motors 0 & 2 > Motors 1 & 3
            int leftBank = this -> motors[0] -> getSpeed();
            int rightBank = this -> motors[1] -> getSpeed();

            //prevent the drone from flipping over
            if(leftBank - rightBank >= 400)
                break;

            //prevent motors from exceeding max/min
	    // making sure that the leftbank is not too high and lowering it down
            if(leftBank + 100 > 2000) leftBank = 1900;
	    // checking the right bank and ensuring it is not too low
            if(rightBank - 100 < 1100) rightBank = 1200;
	    // check to see if the left bank is greather than right bank and if so by 200 steppings then we adjust the motor speeds accordingly
            if(leftBank >= rightBank + 200) {
                this -> setMotorSpeed(0, leftBank + 100);
                this -> setMotorSpeed(2, leftBank + 100);

                this -> setMotorSpeed(3, rightBank - 100);
                this -> setMotorSpeed(1, rightBank - 100);
            }
	    // this case is for when the rightbank is within 100 steppings different from the left bank
            else {
                int diff = 0;
		// if the right bank is greather than the left bank then
		// calculate the difference
                if(rightBank > leftBank)
                    diff = rightBank - leftBank;
                // then use that difference to adjust the motor speeds accordingly
                this -> setMotorSpeed(0, leftBank + diff + 100);
                this -> setMotorSpeed(2, leftBank + diff + 100);

                this -> setMotorSpeed(3, rightBank - 100);
                this -> setMotorSpeed(1, rightBank - 100);
            }
            break;
        }
    // case to move the drone forward
    case '^':
        {
            //move forward, Motors 0 & 1 > Motors 2 & 3
            int frontBank = this -> motors[0] -> getSpeed();
            int backBank = this -> motors[2] -> getSpeed();

            //prevent the drone from flipping over
            if(backBank - frontBank >= 400)
                break;
 
            //prevent motors from exceeding max/min
	    // if the drone's front bank is too low then we increase it
            if(frontBank - 100 < 1100) frontBank = 1200;
	    // if the drone's backbank is to high then we lower it down
            if(backBank + 100 > 2000) backBank = 1900;
	    // if the backbank is greater than the front bank then we adjust the motor speeds accordingly
            if(backBank >= frontBank + 200) {
                this -> setMotorSpeed(0, frontBank - 100);
                this -> setMotorSpeed(1, frontBank - 100);

                this -> setMotorSpeed(2, backBank + 100);
                this -> setMotorSpeed(3, backBank + 100);
            
            }
	    // if the drone's speeds/steppings are within 10%/100 steppings
            else {
                int diff = 0;
		// calculate the difference if front bank is greather than back bank
                if(frontBank > backBank)
                    diff = frontBank - backBank;
		// we do not adjust the frontbank speeds
                this -> setMotorSpeed(0, frontBank - 100);
                this -> setMotorSpeed(1, frontBank - 100);
                // and we use the differences to adjust our calculation to ensure that the drone moves forward
                this -> setMotorSpeed(3, backBank + diff + 100);
                this -> setMotorSpeed(1, backBank + diff + 100);
            }
            break;
        }

    // this is the case for making the drone move backwards, as in
    // move in the reverse direction, all relative to a point of view
    case 'b':
        {
            //move backward, Motors 2 & 3 > Motors 0 & 1
            int frontBank = this -> motors[0] -> getSpeed();
            int backBank = this -> motors[2] -> getSpeed();
   
            //check to prevent the drone from flipping over
            if(frontBank - backBank >= 400) 
                break; 

            // check to prevent motors from exceeding max/min
	    // when the frontbank's steppings is too high we lower it down
            if(frontBank + 100 > 2000) frontBank = 1900;
	    // when the backbank is moving to slowely we increase its speed
            if(backBank - 100 < 1100) backBank = 1200;
	    // when the front bank is slightly or is faster than backbank
	    // we have to set the speeds so it is slightly faster in the front bank
	    // so we move backwards
            if(frontBank >= backBank + 200) {
                this -> setMotorSpeed(0, frontBank + 100);
                this -> setMotorSpeed(1, frontBank + 100);

                this -> setMotorSpeed(2, backBank - 100);
                this -> setMotorSpeed(3, backBank - 100);
            
            }
	    // case were we have the difference is around 100 steps or less (10%)
            else {
                int diff = 0;
		// we check to see if the back is greater than front
                if(backBank > frontBank)
                    diff = backBank - frontBank;
                // here we use that difference to add it to the front bank
                this -> setMotorSpeed(0, frontBank + diff + 100);
                this -> setMotorSpeed(1, frontBank + diff + 100);

                this -> setMotorSpeed(3, backBank - 100);
                this -> setMotorSpeed(1, backBank - 100);
            }
            break;
        }

    // case to turn off the motors, which is when we have placed the drone
    // on departure or arrival destination
    case 'x':
        {
            //turn off all motors
            this -> setAllMotors(0);
            break;
        }

    // this case to set the motors to the minimum speed
    case 'l':
        {
            //set motors to minimum speed
            this -> setAllMotors(1100);
            break;
        }

    case 'h':
        {
            //Expiremental hover!
            if(this -> hover == NULL) {
                this -> hover = new std::thread(&Drone::checkAlt, this);
            }
            else
                this -> hover = NULL;
            
            
            break;
        }
    default: // and of course the default case is to just break
        break;
    }

    return 0;

}

/**
*checkAlt function
*@brief - the function makes the drone hover
*@return - returns null value
*/
void *Drone::checkAlt(){
    //on default mode accelerometer samples at 20hz ie 20 updates to values per second
    while(this -> hover != NULL) {
        printf("Running thread...\n");
	// obtaining the z direction acceleration (that is up and down)
        float z = (float) (this -> acc -> getAccZ()/(16*1000));
        int speed = this -> getAvgMotorSpeed();
	// if the z value is less than zero then re-adjust it to be positive
        if(z < 0) z = (-1) * z;
	// if the value is greater than 1 then it is accelerating  and adjust the speeds so it doesn't fly up
        if(z > 1) {
            if(speed - 25 < 1100) speed = 1125;
            this -> setAllMotors(speed - 25);
        }
	// if the 1 >= speed >= 0, then increase the motor speeds to ensure the drone does not fall down
        else {
            if(speed + 25 > 2000) speed = 1975;
            this -> setAllMotors(speed + 25);
        }
	// time lapse so its not checking all the time for like 0.1 seconds
        timespec timeSleep;
        timeSleep.tv_nsec = 1000000000/10;
        nanosleep(&timeSleep, NULL);
    }
    return NULL;
}

/**
*getGyroData function
*@brief - the getGyroData returns a pointer for the current gyro data of the drone
*@return - the gyro data of the drone
*/
signed short *Drone::getGyroData() {
    // retrive the gyrodata for the drone at the time or request
    this -> gyro_data = this -> acc -> getGyroXYZ();
    return this -> gyro_data;
}

/**
*getAccData function
*@brief - this function returns the acceleration data of the motor
*@return - the acceleration data
*/
signed short *Drone::getAccData(){
    // retrive the acceleration data of the drone
    this -> acc_data = this -> acc -> getAccXYZ();
    return this -> acc_data;
}

/**
*setAllMotors Function
*@brief - the setAllMotors function sets the motor speeds of all the motors to the speed provided by the parameter
*@param speed - this parameter holds the value with which we want to set the speed of th motors of the drone.
*@return - it returns a 0 for success and a -1 for failure which is if one drone motor cannot change speed
*/
int Drone::setAllMotors(int speed){
    if(motors[0] -> setSpeed(speed) < 0 || motors[1] -> setSpeed(speed) < 0 || motors[2] -> setSpeed(speed) < 0 || motors[3] -> setSpeed(speed) < 0) {
        return -1;
    }
    return 0;
}

/**
*setMotorSpeed Function 
*@brief - the setMotorSpeed function sets the motor speed of the motor desired to that speed
*@param motor_id - the motor id of which motor we desire to change its speeds to
*@param speed - the speed of which we want that specific motor set 
*@return - the int representing whether we could set that motor that specific speed or not
*/
int Drone::setMotorSpeed(int motor_id, int speed){
    if(motors[motor_id] -> setSpeed(speed) < 0) {
        return -1;
    }
    return 0;
}

/**
*startDrone function is responsible for taking input and using the appropriate function to perform each task on the drone
*@brief - startDrone takes no parameters but is responsible for taking in input from the server which was sent by the client utilizing the proper funnction to handle each instruction
*/
void Drone::startDrone() {
    char buff[256]; // character buffer to take in an input that is received by the server
    this -> server -> startServer(); // starting the server which will use the buffer to put the received commands
    printf("Drone attempting read...\n");
    // while we have inputs coming in we have to handle the inputs accordingly by using the handelInstruction function
    while(this -> server -> receive(&buff, 1) > 0) {
        handleInstruction(buff[0]);
    }
    printf("Exiting...\n");
}

void Drone::startDrone() {

}

/**
*getAvgMotorSpeed function
*@brief - this function calculates the average motor speed for the drone then returns it
*@return - the average motor speed for the drone
*/
int Drone::getAvgMotorSpeed() {
    return ((this -> motors[0] -> getSpeed() + this -> motors[1] -> getSpeed() + this -> motors[2] -> getSpeed() + this -> motors[3] -> getSpeed()) / 4);
}
