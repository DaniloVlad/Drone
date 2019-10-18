/**
 * Socket Header
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
    protected:
        struct sockaddr_in *addr = NULL;
        int socketfd = 0;
        int socklen = 0;
        int domain = 0;
        int type = 0;
        int protocol = 0;
        int port = 0;
        
    public:
        Socket();
        Socket(int port, char *address, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        Socket(int port, uint32_t address = INADDR_ANY, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        ~Socket();
        

        int setPort(int port) {
            this -> port = htons(port);
            return 0;
        }

        int setHost(const char *host) {
            if(addr != NULL) {
                inet_pton(this -> domain, host, &this -> addr -> sin_addr.s_addr);
                return 0;
            }
            return -1;
        }

        int setType(int type) {
            this -> type = type;
            return 0;
        }

        int setDomain(int domain) {
            this -> domain = domain;
            return 0;
        }

};
#endif
