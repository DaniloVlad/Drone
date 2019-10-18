/**
 * Socket Header
 * */
#ifndef SOCKET_H
#define SOCKET_H
#include <iostream>
#include <string.h>
#include <Socket.h>

class Socket
{
public:
    Socket Socket();
    Socket Socket(int port);
    Socket Socket(int port, struct sockaddr_in address);
    ~Socket();
    send(string data);
    receive();

private:
    int socketfd;
    int new_socket;
    struct sockaddr_in address;
    int port;
    char commands[];
};
#endif