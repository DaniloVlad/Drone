/**
 * Client Header
 * */
#ifndef CLIENT_H
#define CLIENT_H
#include "Drone.h"
#include "Socket.h"

class Client : Socket
{
    
    public:
        Client();
        Client(int port, char *address, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        Client(int port, uint32_t address, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        ~Client() {
            
        }
        int send(void *data, size_t length);
        int receive(void *buffer, size_t length);


};
#endif