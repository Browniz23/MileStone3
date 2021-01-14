//
// Created by oziyo on 06/01/2021.
//

#include <csignal>
#include "Server.h"
#include <setjmp.h>

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

    if (listen(fd, 2) < 0) {
        throw "listen failed";
    }
}

static int val;
static jmp_buf env_buffer;

void jmpfunction(jmp_buf env_buf) {
    cout << "jmp" << endl;
    longjmp(env_buf, 1);
}

static bool stopped = false;      //replace with static?

static void ALARMhandler(int sig) {        //todo: how do i join t? or call stop?
//    signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
    alarm(0);   // need?
    cout << "alrm" << endl;
    stopped = true;
//    jmpfunction( env_buffer );

//    signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
}

void Server::start(ClientHandler& ch)throw(const char*){
    // maybe volatile stopped here?
//    signal(SIGALRM, ALARMhandler);
    t = new thread([&ch, this]() {
//    t = new thread(f);
        /* save calling environment for longjmp */
//        val = setjmp( env_buffer );
        cout << "hey! " << stopped << endl;
        while (!stopped) {
            cout << "waiting for a client" << endl; // todo: cout?

            int iResult;
            struct timeval tv;
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(fd, &rfds);

            tv.tv_sec = (long)3;
            tv.tv_usec = 0;

            iResult = select(fd+1, &rfds, NULL, NULL, &tv);
            if (iResult <= 0) {
                cout << "where is he???" << endl;
                break;
            }

            socklen_t clientSize = sizeof(client);
//            alarm(2); //todo - does not work!!!
            int aClient = accept(fd, (struct sockaddr*)&client, &clientSize);
//            alarm(0);
            if (aClient < 0) {
                throw "accept failed";
            }
            cout << "client connected" << endl;     // todo: cout?
            ch.handle(aClient);
            close(aClient);
      //      sleep(2);                           // todo: make sure get to stop before next while.. no alarm
        }
        close(fd);
        cout << "while-out" << endl;
        cout << "whats next?" << endl;
        cout << t->joinable() << endl;
    });
    cout << " outttt?" << endl;
}

void Server::stop(){             //todo: when succeed stopped=true before third while - V. else -alarm and stuck at end of thread.
    stopped = true;
    cout << "join" << endl;
    t->join(); // do not delete this!
}

Server::~Server() {
    // delete t here?
    delete t;
}


