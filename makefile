CC = g++
ODIR = objs
IDIR = include
BDIR = bin
TDIR = tests

all: Socket.o Server.o Client.o Accelerometer.o Motor.o Drone.o AccelTest MotorTest DroneTest test_client test_server client_ui main

drone: Socket.o Server.o Accelerometer.o Motor.o Drone.o AccelTest MotorTest DroneTest test_server main

client: Socket.o Client.o client_ui test_client

Server.o: $(IDIR)/Socket.h $(IDIR)/Server.h
	$(CC) Server.cpp -c -o $(ODIR)/Server.o

Client.o: $(IDIR)/Socket.h $(IDIR)/Client.h
	$(CC) Client.cpp -c -o $(ODIR)/Client.o

Socket.o: $(IDIR)/Socket.h 
	$(CC) Socket.cpp -c -o $(ODIR)/Socket.o

Accelerometer.o: $(IDIR)/Accelerometer.h $(IDIR)/Configuration.h
	$(CC) Accelerometer.cpp -c -o $(ODIR)/Accelerometer.o

Motor.o: $(IDIR)/Motor.h $(IDIR)/Configuration.h
	$(CC) Motor.cpp -c -o $(ODIR)/Motor.o

Drone.o: $(IDIR)/Drone.h
	$(CC) Drone.cpp -c -o $(ODIR)/Drone.o

AccelTest: $(IDIR)/Accelerometer.h $(IDIR)/Configuration.h
	$(CC) $(TDIR)/Accel_tests.cpp $(ODIR)/Accelerometer.o -lpigpio -o $(BDIR)/AccelTest

MotorTest: $(IDIR)/Motor.h 
	$(CC) $(TDIR)/MotorTest.cpp $(ODIR)/Motor.o -lpigpio -o $(BDIR)/MotorTest

DroneTest: $(IDIR)/Drone.h $(IDIR)/Motor.h $(IDIR)/Accelerometer.h $(IDIR)/Server.h $(IDIR)/Socket.h 
	$(CC) $(TDIR)/DroneTest.cpp $(ODIR)/Drone.o $(ODIR)/Motor.o $(ODIR)/Accelerometer.o $(ODIR)/Server.o $(ODIR)/Socket.o -lpigpio -lpthread -o $(BDIR)/DroneTest

test_client: $(IDIR)/Socket.h $(IDIR)/Client.h
	$(CC) $(TDIR)/test_client.cpp $(ODIR)/Socket.o $(ODIR)/Client.o -o $(BDIR)/test_client

test_server: $(IDIR)/Socket.h $(IDIR)/Server.h
	$(CC) $(TDIR)/test_server.cpp $(ODIR)/Socket.o $(ODIR)/Server.o -o $(BDIR)/test_server

client_ui: $(IDIR)/Client.h $(IDIR)/Socket.h
	$(CC) `pkg-config --cflags gtk+-3.0` -o $(BDIR)/client_ui client_ui.cpp $(ODIR)/Client.o $(ODIR)/Socket.o -lpthread `pkg-config --libs gtk+-3.0`

main: $(IDIR)/Drone.h $(IDIR)/Motor.h $(IDIR)/Accelerometer.h $(IDIR)/Server.h $(IDIR)/Socket.h 
	$(CC) main.cpp $(ODIR)/Drone.o $(ODIR)/Motor.o $(ODIR)/Accelerometer.o $(ODIR)/Server.o $(ODIR)/Socket.o -lpigpio -lpthread -o $(BDIR)/main

clean:
	rm $(ODIR)/* $(BDIR)/*