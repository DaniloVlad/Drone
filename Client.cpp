#include "include/Client.h"

Client::Client() {
    Socket();
    if(connect(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

Client::Client(int port, char *address, int domain, int type, int protocol) {
    Socket(port, address, domain, type, protocol);
    if(connect(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

Client::Client(int port, uint32_t address, int domain, int type, int protocol) {
    Socket(port, address, domain, type, protocol);
    if(connect(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

int Client::receive(void * data, size_t length) {
    return read(this -> socketfd, data, length);
}

int Client::send(void *buffer, size_t length) {
    return write(this -> socketfd, buffer, length);
}
