CC = g++
ODIR = objs
IDIR = include
BDIR = bin
TDIR = tests

all: Socket.o Server.o Client.o test_client test_server client_ui

Server.o: $(IDIR)/Socket.h $(IDIR)/Server.h
	$(CC) Server.cpp -c -o $(ODIR)/Server.o

Client.o: $(IDIR)/Socket.h $(IDIR)/Client.h
	$(CC) Client.cpp -c -o $(ODIR)/Client.o

Socket.o: $(IDIR)/Socket.h
	$(CC) Socket.cpp -c -o $(ODIR)/Socket.o

test_client: $(IDIR)/Socket.h $(IDIR)/Client.h
	$(CC) $(TDIR)/test_client.cpp $(ODIR)/Socket.o $(ODIR)/Client.o -o $(BDIR)/test_client

test_server: $(IDIR)/Socket.h $(IDIR)/Server.h
	$(CC) $(TDIR)/test_server.cpp $(ODIR)/Socket.o $(ODIR)/Server.o -o $(BDIR)/test_server

client_ui: 
	$(CC) `pkg-config --cflags gtk+-3.0` -o ui/example-0 ui/ui-test1.cpp `pkg-config --libs gtk+-3.0`
clean:
	rm $(ODIR)/* $(BDIR)/*