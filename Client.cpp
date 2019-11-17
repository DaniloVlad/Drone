#include "include/Client.h"

// Constructs the client without any specific paramater input for it's components.
Client::Client() {
    Socket();
  	// Attempts to connect to the client socket after creation, and outputs an error message for exiting if the attempt fails.
    if(connect(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

// Constructs the client with specific paramater input for it's components, and a char address type.
Client::Client(int port, char *address, int domain, int type, int protocol) {
    Socket(port, address, domain, type, protocol);
    // Attempts to connect to the client socket after creation, and outputs an error message for exiting if the attempt fails.
    if(connect(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

// Constructs the client with specific paramater input for it's components, and a uint32_t address type.
Client::Client(int port, uint32_t address, int domain, int type, int protocol) {
    Socket(port, address, domain, type, protocol);
    // Attempts to connect to the client socket after creation, and outputs an error message for exiting if the attempt fails.
    if(connect(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

// Returns the information read in to the client socket.
int Client::receive(void * data, size_t length) {
    return read(this -> socketfd, data, length);
}

// Returns the information to write in to the client socket.
int Client::send(void *buffer, size_t length) {
    return write(this -> socketfd, buffer, length);
}
