/**
 * @include Client header
 * */
#ifndef CLIENT_H
#define CLIENT_H
#include "Socket.h"

// Socket class that allows sending and receiving data to the client class.
class Client : public Socket
{
    public:
      	// Specifies a base constructor, constructors with params and a deconstructor function.
        Client();
        Client(int port, char *address, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        Client(int port, uint32_t address, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        ~Client();
        
        // Functions to send and recieve streams of data for coordination.
        int send(void *data, size_t length);
        int receive(void *buffer, size_t length);
};
#endif
