#include "include/Socket.h"

// Constructs the socket without any specific paramater input for it's components.
Socket::Socket() {
  
  	// Sets all the constructor variables.
    this -> addr = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    this -> socklen = sizeof(struct sockaddr_in);
    this -> port = htons(DEFAULT_PORT);
    this -> domain = DEFAULT_DOMAIN;
    this -> protocol = DEFAULT_PROTOCOL;
    this -> type = DEFAULT_TYPE;

  	//
    memset(this -> addr, 0, this -> socklen);
    
    this -> addr -> sin_addr.s_addr = DEFAULT_ADDR;
    this -> addr -> sin_family = this -> domain;
    this -> addr -> sin_port = this -> port;

  	// Error check - attempting to create the socket. If failed, sends an error message and exit.
    if((this -> socketfd = socket(this -> domain, this -> type, this -> protocol)) == 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }
}   

// Constructs the socket with specific paramater input for it's components, using the char address.
Socket::Socket(int port, char *address, int domain, int type, int protocol) {
  
   	// Sets all the constructor variables. 
    this -> addr = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    this -> socklen = sizeof(struct sockaddr_in);
    this -> port = htons(port);
    this -> type = type;
    this -> protocol = protocol;
    
  	//
    memset(this -> addr, 0, this -> socklen);
    inet_pton(this -> domain, address, &this -> addr -> sin_addr.s_addr);

    this -> addr -> sin_family = this -> domain;
    this -> addr -> sin_port = this -> port;

    // Error check - attempting to create the socket. If failed, sends an error message and exit.
    if((this -> socketfd = socket(this -> domain, this -> type, this -> protocol)) == 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }
}

// Constructs the socket with specific paramater input for it's components, using the uint32_t address.
Socket::Socket(int port, uint32_t address, int domain, int type, int protocol) {
  
    // Sets all the constructor variables.
    this -> addr = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    this -> socklen = sizeof(struct sockaddr_in);
    this -> port = htons(port);
    this -> type = type;
    this -> domain = domain;
    this -> protocol = protocol;

  	//
    memset(this -> addr, 0, this -> socklen);

    this -> addr -> sin_family = this -> domain;
    this -> addr -> sin_port = this -> port;
    this -> addr -> sin_addr.s_addr = address;
  
    // Error check - attempting to create the socket. If failed, sends an error message and exit.
    if((this -> socketfd = socket(this -> domain, this -> type, this -> protocol)) == 0) {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }
}

// Deconstructor method for the socket.
Socket::~Socket() {
    if(addr != NULL) 
        free(this -> addr);
    close(this -> socketfd);
}
