#include "include/Server.h"

Server::Server() : Socket() {
    this -> drone = NULL;
    this -> client_addr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
    this -> client_socklen = sizeof(sockaddr_in);

    memset(this -> client_addr, 0, sizeof(sockaddr_in));

    int opt = 1;
    
    if(setsockopt(this -> socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("Couldn't set socket options");
        exit(EXIT_FAILURE);
    }

    if(bind(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't bind socket!");
        exit(EXIT_FAILURE);
    }

    if(listen(this -> socketfd, 1) < 0) {
        perror("Couldn't listen on socket");
        exit(EXIT_FAILURE);
    }
}

Server::Server(Drone *drone) : Socket() {
    this -> drone = drone;
    this -> client_addr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
    this -> client_socklen = sizeof(sockaddr_in);

    memset(this -> client_addr, 0, sizeof(sockaddr_in));

    int opt = 1;

    if(setsockopt(this -> socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("Couldn't set socket options");
        exit(EXIT_FAILURE);
    }

    if(bind(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't bind socket!");
        exit(EXIT_FAILURE);
    }

    if(listen(this -> socketfd, 1) < 0) {
        perror("Couldn't listen on socket");
        exit(EXIT_FAILURE);
    }
}

Server::Server(Drone *drone, int port, char *address, int domain, int type, int protocol) : Socket(port, address, domain, type, protocol) {
    this -> drone = drone;
    this -> client_addr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
    this -> client_socklen = sizeof(sockaddr_in);

    memset(this -> client_addr, 0, sizeof(sockaddr_in));

    int opt = 1;

    if(setsockopt(this -> socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("Couldn't set socket options");
        exit(EXIT_FAILURE);
    }
    
    if(bind(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't bind socket!");
        exit(EXIT_FAILURE);
    }

    if(listen(this -> socketfd, 1) < 0) {
        perror("Couldn't listen on socket");
        exit(EXIT_FAILURE);
    }
}

Server::Server(Drone *drone, int port, uint32_t address, int domain, int type, int protocol) : Socket(port, address, domain, type, protocol) {
    
    this -> drone = drone;
    this -> client_addr = (struct sockaddr_in *) malloc(sizeof(sockaddr_in));
    this -> client_socklen = sizeof(sockaddr_in);

    memset(this -> client_addr, 0, sizeof(sockaddr_in));

    int opt = 1;

    if(setsockopt(this -> socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("Couldn't set socket options");
        exit(EXIT_FAILURE);
    }
    
    if(bind(this -> socketfd, (sockaddr *) this -> addr, this -> socklen) < 0) {
        perror("Couldn't bind socket!");
        exit(EXIT_FAILURE);
    }

    if(listen(this -> socketfd, 1) < 0) {
        perror("Couldn't listen on socket");
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    if(this -> client_addr) free(this -> client_addr);
    printf("Closing server socket...\n");
    close(new_socket);
}

int Server::startServer() {
    if((this -> new_socket = accept(this -> socketfd, (sockaddr *) this -> client_addr, (socklen_t *) & this -> client_socklen)) < 0) {
        perror("Error accepting connection!");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int Server::send(void *data, size_t length) {
    return write(this -> new_socket, data, length);
}

int Server::receive(void * buffer, size_t length) {
    return read(this -> new_socket, buffer, length);
}

