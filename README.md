**Please find related documents in the documents/ folder.**

    - UML diagram can be found there as well
    - Found in the test_screenshots/ folder is proof of working utilities.
	- Additonally, source files for these tests can be found in the tests/ folder

Checkout the [documentation!](https://danilovlad.github.io/Drone/) 

Video Of the Drone working: [Here!](https://drive.google.com/file/d/1PWa7tf55ajyGLO5VAZWS5-eSpB_HxBNT/view?usp=sharing)

# Setting up the pi:

    sudo apt-get install pigpio

Next configure the pi as a [wireless access point](https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md)

(Set the IP to 192.168.4.1 for the code to work out of box)

# Configuration.h

    In the include/ this file should be edited to the appropriate pins and settings for your hookup!

# Compiling

On Drone (raspberry pi):

    make drone
    sudo ./bin/main

On Client (any linux machine with UI):

    make client
    ./bin/client_ui
    

# Testing

sample outputs availible in [test_screenshots](https://github.com/DaniloVlad/Drone/tree/master/test_screenshots)
Testing socket:

    ./bin/test_server
    ./bin/test_client

Testing UI:

    ./bin/client_ui

Testing Accelerometer:

    ./bin/Accel_tests.cpp
    or
    python3 tests/Accel_tests.py

Testing Motors:

    ./bin/MotorTest
    or
    python3 tests/testMotor.py

