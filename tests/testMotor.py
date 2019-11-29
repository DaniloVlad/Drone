import time
import pigpio

pi = pigpio.pi()
pi.set_mode(4, pigpio.OUTPUT)
input("Disconnect the battery and press enter")
print("Setting max throttle")

pi.set_servo_pulsewidth(4, 2000)
input("Connect the battery and press enter")
time.sleep(1)
pi.set_servo_pulsewidth(4, 1000)

time.sleep(1)

input("Press enter to complete setup")

x = 1
while (x >= 0):
	x = int(input("Enter a value (1000 min, 2000 max): "))
	pi.set_servo_pulsewidth(4, x)
