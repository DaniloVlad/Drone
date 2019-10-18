/**
 * Server Header
 * */
#ifndef SERVER_H
#define SERVER_H
#include "Drone.h"
#include "Socket.h"

class Server : Socket
{
public:
    int createServer(struct sockaddr client_addr);
    void startServer();
    int setDrone(Drone drone);
    Drone getDrone();

private:
    Drone drone;
    struct sockaddr client_addr: 
};
#endif