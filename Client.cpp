/**
 * @class Client
 * @brief generates an instance of a client to connect to the PI
 * */


#include "include/Client.h"

/**
 * Function: Client Constructor
 * @brief - Constructs the client without any specific paramater input for it's components.
 */  
Client::Client() : Socket() {
  	// Attempts to connect to the client socket after creation, and outputs an error message for exiting if the attempt fails.
    if(connect(this -> socketfd, (struct sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

/**
 *  Function: Client Constructor
*   @brief - Constructs the client with specific paramater input for it's components, and a char address type.
*   @param port - port for the client.
*   @param address - address for the client. 
*   @param domain - domain for the client.
*   @param type - type of data for the client.
*   @param protocol - protocol the client is using.
*/  
Client::Client(int port, char *address, int domain, int type, int protocol) : Socket(port, address, domain, type, protocol) {
    
    // Attempts to connect to the client socket after creation, and outputs an error message for exiting if the attempt fails.
    if(connect(this -> socketfd, (struct sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

/* Function: Client Constructor
*   @brief - Constructs the client with specific paramater input for it's components, and a uint32_t address type.
*   @param port - port for the client.
*   @param address - address for the client. 
*   @param domain - domain for the client.
*   @param type - type of data for the client.
*   @param protocol - protocol the client is using.
*/  
Client::Client(int port, uint32_t address, int domain, int type, int protocol) : Socket(port, address, domain, type, protocol) {
    // Attempts to connect to the client socket after creation, and outputs an error message for exiting if the attempt fails.
    if(connect(this -> socketfd, (struct sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't connect client socket");
        exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    printf("Closing socket\n");
}

/* Function: Recieve
*   @brief - Returns the information read in to the client socket.
*   @param data - the data to be recieved by the client.
*   @param length - the size/length of the data.
*/  
int Client::receive(void * data, size_t length) {
    return read(this -> socketfd, data, length);
}

/* Function: Send
*   @brief - Returns the information to write in to the client socket.
*   @param buffer - the data to be sent by the client. 
*   @param length - the size/length of the data. 
*/  
int Client::send(void *buffer, size_t length) {
    return write(this -> socketfd, buffer, length);
}
