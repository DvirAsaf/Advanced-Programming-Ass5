/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef SERVER_H_
#define SERVER_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include "commands.h"
#include "CLI.h"
using namespace std;
// ClientHandler interface
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};

//socketDIO for reading and writing from and into sockets
class socketDIO : public DefaultIO {
    int socketFD;
public:
    //constructor of socketDIO getting fileDescriptor
    socketDIO(int fd) {
        this->socketFD = fd;
    }
    void write(string text) override {
        const char *charText = text.c_str();
        send(socketFD, charText, text.size(), 0);
    }
    void write(float f) override {
        char charText[sizeof(f) + 1];
        sprintf(charText, "%f", f);
        send(socketFD, charText, sizeof(charText), 0);
    }
    string read() override {
        string serverInput = "";
        char c = 0;
        recv(socketFD, &c, sizeof(char), 0);
        while (c != '\n') {
            serverInput += c;
            recv(socketFD, &c, sizeof(char), 0);
        }
        return serverInput;
    }
    void read(float *f) override {
        recv(socketFD, f, sizeof(*f), 0);
    }
};
// AnomalyDetectionHandler class - handle Client
class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        socketDIO dio(clientID);
        CLI cli(&dio);
        cli.start();
    }
};
class Server {
    // the thread to run the start() method in
    thread *t;
public:
    int fileDescriptor;
    struct sockaddr_in server, client;
    bool acceptConnection;
    Server(int port) throw(const char *);
    virtual ~Server();
    void start(ClientHandler &ch) throw(const char *);
    void stop();
};
#endif /* SERVER_H_ */