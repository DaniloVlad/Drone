#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include "../include/Motor.h"


int main() {
	if(gpioInitialise() < 0) {
		printf("Error init gpio library!\n");
		return -1;
	}
	char buff[100];
	int x = 0;
	Motor *m1 = new Motor(4);
	printf("Disconnect the battery and press enter\n");
	fgets(buff, 100, stdin);
	m1 -> setSpeed(2000);
	printf("Connect the battery and press enter\n");
	fgets(buff, 100, stdin);
	sleep(2);
	m1 -> setSpeed(1000);
	sleep(2);
	printf("Armed!\n");
	while(x >= 0) {
		printf("Enter a speed: ");
		scanf(" %d", &x);
		printf("Entered: %d", x);
		m1 -> setSpeed(x);
	}
	return 0;

}
