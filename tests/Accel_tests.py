import pigpio
import struct
import time

BUS = 1
ADDR = 0x69

X_ACC = 0x2d
Y_ACC = 0x2f
Z_ACC = 0x31
SENS_ACC = 16 * 1000 #convert values from mg to g
RUNTIME = 60

pi = pigpio.pi()

i2c = pi.i2c_open(BUS, ADDR)

if i2c >= 0:
	pi.i2c_write_byte_data(i2c, 0x06, 128)
	time.sleep(1)
	pi.i2c_write_byte_data(i2c, 0x06, 0)
	pi.i2c_write_byte_data(i2c, 0x07, 0)
	pi.i2c_read_i2c_block_data(i2c, 0x7f, 0x20)
	pi.i2c_write_byte_data(i2c, 0x14, 7)
	pi.i2c_write_byte_data(i2c, 0x15, 3)
	pi.i2c_write_byte_data(i2c, 0x7f, 0)
	read = 0
	start_time = time.time()

	while(time.time() -start_time) < RUNTIME:
		s, accel = pi.i2c_read_i2c_block_data(i2c, X_ACC, 6)
		if s >= 0:
			(x, y, z) = struct.unpack(">3h", accel)
			print("X = {}g Y = {}g Z = {}g ".format(float(x)/SENS_ACC, y/SENS_ACC, z/SENS_ACC))
			print("Bytes read: " + str(s))
		time.sleep(0.5)

