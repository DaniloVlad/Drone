#include "include/Socket.h"

Socket::Socket() {
    this -> addr = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    this -> socklen = sizeof(struct sockaddr_in);

    this -> port = htons(DEFAULT_PORT);
    this -> domain = DEFAULT_DOMAIN;
    this -> protocol = DEFAULT_PROTOCOL;
    this -> type = DEFAULT_TYPE;

    memset(this -> addr, 0, this -> socklen);
    
    this -> addr -> sin_addr.s_addr = DEFAULT_ADDR;
    this -> addr -> sin_family = this -> domain;
    this -> addr -> sin_port = this -> port;

    
    if((this -> socketfd = socket(this -> domain, this -> type, this -> protocol)) == 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }

}   


Socket::Socket(int port, char *address, int domain, int type, int protocol) {
    this -> addr = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    this -> socklen = sizeof(struct sockaddr_in);

    this -> port = htons(port);
    this -> type = type;
    this -> protocol = protocol;
    
    memset(this -> addr, 0, this -> socklen);
    inet_pton(this -> domain, address, &this -> addr -> sin_addr.s_addr);

    this -> addr -> sin_family = this -> domain;
    this -> addr -> sin_port = this -> port;

    if((this -> socketfd = socket(this -> domain, this -> type, this -> protocol)) == 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }
}

Socket::Socket(int port, uint32_t address, int domain, int type, int protocol) {
    this -> addr = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    this -> socklen = sizeof(struct sockaddr_in);

    this -> port = htons(port);
    this -> type = type;
    this -> domain = domain;
    this -> protocol = protocol;

    memset(this -> addr, 0, this -> socklen);

    this -> addr -> sin_family = this -> domain;
    this -> addr -> sin_port = this -> port;
    this -> addr -> sin_addr.s_addr = address;

    if((this -> socketfd = socket(this -> domain, this -> type, this -> protocol)) == 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }
}

Socket::~Socket() {
    if(addr != NULL) 
        free(this -> addr);
    close(this -> socketfd);
    
    
}
