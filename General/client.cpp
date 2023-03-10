// Client side C/C++ program to demonstrate Socket
// programming
#include <utility>
#include "Client.h"
#include <stdio.h>

/**
 * Checks validity of inputed distance function. Throws exception if invalid
 */
void Client::checkDistanceFunc(std::string &distance) {
    if (distance == "MAN" || distance == "AUC" || distance == "CHE" || distance == "MIN"
        || distance == "CHB") {
        return;
    } else {
        throw std::ios_base::failure("invalid input");
    }
}

/**
 * Check validity of uder input. Throws exception if invalid
 */
void Client::checkValidInput(std::string &input) {
    std::vector<std::string> elems;
    string vector;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        if (item == "") {
            throw std::ios_base::failure("invalid input");
        }
        elems.push_back(item);
    }
    if (elems.size() < 3) {
        throw std::ios_base::failure("invalid input");
    }
    for (int i = 0; i < elems.size() - 2; i++) {
        if (i == elems.size() - 3)
            vector += elems[i];
        else
            vector += elems[i] + ",";
    }
    Vector v(vector);
    checkDistanceFunc(elems[elems.size() - 2]);
    // check if the k is a number
    if (std::stoi(elems[elems.size() - 1]) < 0) {
        throw std::ios_base::failure("invalid input");
    }
}

/**
 * converts string into valid int for port
 * @return valid port number, or -1 if none exists
 */
unsigned short Client::getPort(string portStr) {
    int port;
    try {
        port = std::stoi(portStr);
    }
    catch (std::invalid_argument const &ex) {
        std::cout << "invalid input" << std::endl;
        return -1;
    }
    if (port > 65535 || port < 1) {
        std::cout << "Invalid port number. Exiting program" << std::endl;
        return -1;
    }
    return (unsigned short) port;
}

/**
 * Connect to socket, throw exception in case of failure
 * @return file descriptor of connected socket 
 */
int Client::connectSock(unsigned short portNo) {
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::ios_base::failure("Socket creation error");
    }
    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNo);
//    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
//        throw; ////TODO
//    }
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror(NULL);
        throw std::ios_base::failure("\nConnection Failed");
    }
    return sock;
}

Client::Client(std::string ipStr, std::string portStr) {
    port = getPort(std::move(portStr));
    if (port < 0) {
        throw std::ios_base::failure("Error getting port number");
    }
    ip = ipStr.c_str();
}

/**
 * main function
 */
void Client::run() {
    // connect to socket
    int sock;
    try {
        sock = connectSock(port);
    } catch (std::ios_base::failure const &ex) {
        throw ex;
    }
    // get input from user
    SocketIO socketIO(sock);
    ClientCLI clientCLI(&socketIO);
    clientCLI.start();
}

