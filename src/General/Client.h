#ifndef ASS4_CLIENT_H
#define ASS4_CLIENT_H

#include <algorithm>
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "ClientCLI/ClientCLI.h"
#include "IO/SocketIO.h"
#include "Vector.h"

class Client {
    int port;
    const char *ip;
    struct sockaddr_in serv_addr{};
    void checkDistanceFunc(std::string &distance);
    void checkValidInput(std::string &input);
    int getPort(string portStr);
    int connectSock();
public:
    void run();
    Client(std::string ipStr, std::string portStr);
    ~Client() = default;

};

#endif //ASS4_CLIENT_H