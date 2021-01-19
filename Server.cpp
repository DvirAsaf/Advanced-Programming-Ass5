/**
 * @author : Dvir Asaf 313531113.
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include "Server.h"

/* constructor of Server - opening a socket, binding to a port and listen */
Server::Server(int port) throw(const char *) {
    //opening a socket - checking if succeed
    fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (fileDescriptor < 0) {
        throw "error in socket";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    //binding port to socket - checking if succeed
    if (bind(fileDescriptor, (struct sockaddr *) &server, sizeof(server)) < 0) {
        throw "error in bind";
    }
    //listen configuration
    if (listen(fileDescriptor, 3) < 0) {
        throw "error in listen";
    }
}
/* starting server  in thread. waiting for client to connect and than handle acoording
 * to ClientHandler. */
void Server::start(ClientHandler &ch) throw(const char *) {
    this->acceptConnection = true;
    t = new thread([&ch, this]() {
        while (acceptConnection) {
            socklen_t clientSize = sizeof(client);
            int clientFD = accept(fileDescriptor, (struct sockaddr *) &client, &clientSize);
            if (clientFD < 0) {
                throw "error in acceptConnection";
            }
            ch.handle(clientFD);
            close(clientFD);
        }
    });
}
/*stopping server - stopping loop and than waiting for thread to finsih.
 * closing file Descriptor. */
void Server::stop() {
    acceptConnection = false;
    t->join(); // do not delete this!
    close(fileDescriptor);
}
Server::~Server() {
}