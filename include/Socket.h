/**
 * @include Socket Header
 * */
#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define DEFAULT_ADDR INADDR_ANY
#define DEFAULT_PORT 8080
#define DEFAULT_DOMAIN AF_INET
#define DEFAULT_TYPE SOCK_STREAM
#define DEFAULT_PROTOCOL 0

class Socket
{
  	// Defines all the variables that will be used in the construction of the server object.
    protected:
        struct sockaddr_in *addr = NULL;
        int socketfd = 0;
        int socklen = 0;
        int domain = 0;
        int type = 0;
        int protocol = 0;
        int port = 0;
        
  	// Socket functions.    
    public:
        // Specifies a base constructor, constructors with different address params and a deconstructor function.
        Socket();
        Socket(int port, char *address, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        Socket(int port, uint32_t address = INADDR_ANY, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        ~Socket();
        
		// Setter function for the socket port. Returns 0 if successful.
        int setPort(int port) {
            this -> port = htons(port);
            return 0;
        }

		// Setter function for the socket host. Returns 0 if successful, and -1 if unsuccesful.
        int setHost(const char *host) {
            if(addr != NULL) {
                inet_pton(this -> domain, host, &this -> addr -> sin_addr.s_addr);
                return 0;
            }
            return -1;
        }

		// Setter function for the socket type. Returns 0 if successful.
        int setType(int type) {
            this -> type = type;
            return 0;
        }

		// Setter function for the socket domain. Returns 0 if successful.
        int setDomain(int domain) {
            this -> domain = domain;
            return 0;
        }

};
#endif
