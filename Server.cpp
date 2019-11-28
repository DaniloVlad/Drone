#include "include/Server.h"

/* Function: Socket Constructor
*   @breif - Constructs the server socket without any specific paramater input for it's components.
*/  
Server::Server() : Socket() {
    printf("Creating server....\n");
    this -> client_addr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
    this -> client_socklen = sizeof(sockaddr_in);
    memset(this -> client_addr, 0, sizeof(sockaddr_in));
    int opt = 1;
    
  	// Error check - attempting to set socket options. If failed, sends an error message and exit.
    if(setsockopt(this -> socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("Couldn't set socket options");
        exit(EXIT_FAILURE);
    }
  
  	// Error check - attempting to bind the socket. If failed, sends an error message and exit.
    if(bind(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't bind socket!");
        exit(EXIT_FAILURE);
    }
  
  	// Error check - attempting to listen to the socket. If failed, sends an error message and exit.
    if(listen(this -> socketfd, 1) < 0) {
        perror("Couldn't listen on socket");
        exit(EXIT_FAILURE);
    }
}


/* Function: Socket Constructor
*   @breif - Constructs the server socket with specific paramater input for it's components, using the char address.
*   @param port - port for the socket.
*   @param address - address for the socket.. 
*   @param domain - domain for the socket.
*   @param type - type of data for the socket.
*   @param protocol - protocol the socket is using.
*/  
Server::Server(int port, char *address, int domain, int type, int protocol) : Socket(port, address, domain, type, protocol) {
    
    this -> client_addr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
    this -> client_socklen = sizeof(sockaddr_in);
    memset(this -> client_addr, 0, sizeof(sockaddr_in));
    int opt = 1;

    // Error check - attempting to set socket options. If failed, sends an error message and exit.
    if(setsockopt(this -> socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("Couldn't set socket options");
        exit(EXIT_FAILURE);
    }
    
  	// Error check - attempting to bind the socket. If failed, sends an error message and exit.  
    if(bind(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't bind socket!");
        exit(EXIT_FAILURE);
    }
  
  	// Error check - attempting to listen to the socket. If failed, sends an error message and exit.
    if(listen(this -> socketfd, 1) < 0) {
        perror("Couldn't listen on socket");
        exit(EXIT_FAILURE);
    }
}

/* Function: Socket Constructor
*   @breif - Constructs the server socket with specific paramater input for it's components, using the uint32_t address.
*   @param port - port for the socket.
*   @param address - address for the socket.. 
*   @param domain - domain for the socket.
*   @param type - type of data for the socket.
*   @param protocol - protocol the socket is using.
*/  
Server::Server(int port, uint32_t address, int domain, int type, int protocol) : Socket(port, address, domain, type, protocol) {
    printf("Creating server....\n");
    this -> client_addr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
    this -> client_socklen = sizeof(sockaddr_in);
    memset(this -> client_addr, 0, sizeof(sockaddr_in));

    int opt = 1;

    // Error check - attempting to set socket options. If failed, sends an error message and exit.
    if(setsockopt(this -> socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("Couldn't set socket options");
        exit(EXIT_FAILURE);
    }
    
  	// Error check - attempting to bind the socket. If failed, sends an error message and exit.  
    if(bind(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't bind socket!");
        exit(EXIT_FAILURE);
    }

    // Error check - attempting to listen to the socket. If failed, sends an error message and exit.
    if(listen(this -> socketfd, 1) < 0) {
        perror("Couldn't listen on socket");
        exit(EXIT_FAILURE);
    }
}

/* Function: Server Deconstructor
*   @breif - Deconstructor method for the server socket.
*/  
Server::~Server() {
    if(this -> client_addr) free(this -> client_addr);
    printf("Closing server socket...\n");
    close(new_socket);
}

/* Function: startServer
*   @breif - Starter function that attempts to start the server. If failed, sends an error message and exit.
*/  
int Server::startServer() {
    printf("Server waiting on connection...\n");
    if((this -> new_socket = accept(this -> socketfd, (sockaddr *) this -> client_addr, (socklen_t *) & this -> client_socklen)) < 0) {
        perror("Error accepting connection!");
        exit(EXIT_FAILURE);
    }
    return 0;
}

/* Function: send
*   @breif - Returns the information read in to the server socket.
*   @param data - the data to be recieved by the server.
*   @param length - the size/length of the data.
*/  
int Server::send(void *data, size_t length) {
    return write(this -> new_socket, data, length);
}

/* Function: receive
*   @breif - Returns the information to write in to the server socket.
*   @param buffer - the data to be sent by the server. 
*   @param length - the size/length of the data. 
*/  
int Server::receive(void * buffer, size_t length) {
    printf("Server waiting for data...\n");
    return read(this -> new_socket, buffer, length);
}

