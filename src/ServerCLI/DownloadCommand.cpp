#include "ServerCLI/DownloadCommand.h"
#include <unistd.h>
#include <sys/socket.h>
#include <thread>
#include "General/Server.h"
#include "ServerCLI/Uploader.h"

void DownloadCommand::execute() {
    if (database->isFilesUnloaded() && database->getClassfications() == "") {
        dio->write("please upload data\nplease classify the data\n");
        dio->read();
        return;
    }
    if (database->getClassfications().empty()) {
        dio->write("please classify the data\n");
        dio->read();
        return;
    }
    if (database->isFilesUnloaded()) {
        dio->write("please upload data\n");
        dio->read();
        return;
    }
    // if all is good, bind to port
    std::cout << "Creating new socket" << std::endl;
    int newSock = Server::bindSock(0);
    std::cout << "Sending port to client" << std::endl;
    struct sockaddr_in addr;
    socklen_t addrlen;
    if (getsockname(newSock, (struct sockaddr *)(&addr), &addrlen) < 0) {
        std::cerr << "Error starting new socket" << std::endl;
        return;
    }
    // send port number to client
    unsigned short port = ntohs(addr.sin_port);
    std::cout << "Telling client to connect to port " << port << std::endl;
    dio->write(std::to_string(port));
    dio->read();
    std::cout << "Waiting for connection from client" << std::endl;
    if (listen(newSock, 1) < 0) {
        throw std::ios_base::failure("Error listening to a socket");
    }
    struct sockaddr_in client_sin{};
    unsigned int addr_len = sizeof(client_sin);
    int clientSock = accept(newSock, (struct sockaddr *)&client_sin, &addr_len);
    if (clientSock < -1) {
        throw std::ios_base::failure("Error accepting client");
    }
    std::cout << "Spawning uploader thread" << std::endl;
    SocketIO *socket = new SocketIO(clientSock);
    Uploader u;
    std::thread thread1(u, socket, database);
    thread1.detach();
}

DownloadCommand::DownloadCommand(DefaultIO *pIo, Database *pDatabase) {
    dio = pIo;
    database = pDatabase;
}