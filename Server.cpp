// 316482355 Oz Browner

#include <csignal>
#include "Server.h"
#include <setjmp.h>


#define ACCEPT_TIME_OUT 3
#define CLIENTS_ALLOWED 1

Server::Server(int port)throw (const char*) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        throw "bind failed";
    }

    // 1 client is allowed each time
    if (listen(fd, CLIENTS_ALLOWED) < 0) {
        throw "listen failed";
    }
}

// true when server need to shut down, and stop thread loop.
volatile bool stopped = false;

/**
 * using select to check if server socket didnt got request within ACCEPT_TIME_OUT seconds.
 * @param fd - socket number;
 * @return true if timedout, else false.
 */
bool isTimeOuted(int fd) {
    int iResult;
    struct timeval tv;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = (long)ACCEPT_TIME_OUT;
    tv.tv_usec = 0;
    iResult = select(fd+1, &rfds, NULL, NULL, &tv);
    if (iResult > 0) {
        return false;
    }
    return true;
}

void Server::start(ClientHandler& ch)throw(const char*){
    // thread runs and wait for clients until server stop.
    t = new thread([&ch, this]() {
        while (!stopped) {
            cout << "waiting for a client" << endl; // todo: cout?
            if (!isTimeOuted(fd)) {
                socklen_t clientSize = sizeof(client);
                int aClient = accept(fd, (struct sockaddr*)&client, &clientSize);
                if (aClient < 0) {
                    throw "accept failed";
                }
                cout << "client connected" << endl;     // todo: cout?
                ch.handle(aClient);
                close(aClient);
            }
        }
        close(fd);
    });
}

void Server::stop(){
    stopped = true; // thread finishes after current client
    t->join(); // do not delete this!
}

Server::~Server() {
    delete t;
}


