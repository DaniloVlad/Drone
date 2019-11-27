#include "../include/Client.h"
#include <sys/socket.h>

int main() {
    char buff[3] = {0};
    char msg[6] = "Hello";
    char host[13] = "192.168.0.10";
    printf("Connecting client to %s:%d\n", host, DEFAULT_PORT);
    Client *test = new Client(DEFAULT_PORT, host);
    printf("Client sending: %s\n", msg);
    test -> send((void *) msg, 5);
    printf("Message sent. Client waiting for response..\n");
    test -> receive(buff, 3);
    printf("Client received: %s\n", buff);
    delete test;
    return 0;
}
