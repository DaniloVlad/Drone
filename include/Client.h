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
    int createClient(struct sockaddr server_addr); 
    void startClient();

private:
    struct sockaddr client_addr;                                                                 private : Drone drone;
};
#endif