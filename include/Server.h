/**
 * Server Header
 * */
#ifndef SERVER_H
#define SERVER_H
#include "Socket.h"

class Server : public Socket
{
    private:
        struct sockaddr_in *client_addr = NULL;
        int new_socket = 0;
        int client_socklen = 0;

    public:
        Server();

        Server(int port, char *address, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);

        Server(int port, uint32_t address = INADDR_ANY, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);

        ~Server();
        int startServer();

        int send(void *data, size_t length);
        int receive(void *buffer, size_t length);
};
#endif
