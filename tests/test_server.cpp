#include "../include/Server.h"

int main() {
    char buff[256] = {0};
    printf("Creating server on 0.0.0.0:8080\n");
    Server *t1 = new Server();
    t1 -> startServer();
    printf("Server started. Waiting for client data...\n");
    t1 -> receive(buff, 256);
    printf("Got %s\n", buff);
    t1 -> send((void *) "ok", 2);
    printf("Sent back: ok\n");
    delete t1;
    return 0;
}