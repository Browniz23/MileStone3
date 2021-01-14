//
// Created by oziyo on 06/01/2021.
//

//g++ -pthread -std=c++11 anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h timeseries.h SimpleAnomalyDetector.h timeseries.cpp SimpleAnomalyDetector.cpp HybridAnomalyDetector.h HybridAnomalyDetector.cpp CLI.cpp CLI.h minCircle.cpp minCircle.h commands.h StandardIO.cpp StandardIO.h MainTrain.cpp Server.cpp Server.h

#ifndef MILESTONE3_SERVER_H
#define MILESTONE3_SERVER_H

#include "CLI.h"
#include <iostream>
#include <pthread.h>
#include <thread>
//#include <winsock2.h>//#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <signal.h>
#include <time.h>
#include <string.h>



using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class SocketIO:public DefaultIO {
    int fd;

    string strFloatCut(string s) {
        int len_without_zeros = s.length();
        for (int i = s.length()-1; i >= 1; i--) {
            if (s[i] == '0') {
                len_without_zeros--;
            } else {
                break;
            }
        }
        return s.substr(0, len_without_zeros);
    }
public:
    SocketIO(int fileDescriptor) {
        fd = fileDescriptor;
    }

    virtual string read() {
        char c = 0;
        string s = "";
        recv(fd,&c,1,0);        // added 0 at end.???               //todo @@@@@@ server stuck here sec time (A,B,C)
        while (c != '\n') {                //todo ONLY for windows add '/r'!!!!!!!!!!!!!!!!!!1 changed :)
            s += c;
            recv(fd,&c,1,0);
        }                               // todo need add \n?
        return s;
    }
    virtual void write(string text) {
        send(fd, text.c_str(), text.length(), 0);          // make sure text.length same length as c_str
        //write(fd, "\n", 1);
    }

    virtual void write(float f) {
        //send(fd, &f, sizeof(float),0);                 // like write?           //todo: not deal with endians!
        string s = to_string(f);        // todo works?
        //s += "\n"; // todo needed?
        s = strFloatCut(s);         // todo make sure works
        write(s);
    }
    virtual void read(float* f) {
        //read(fd, &f, sizeof(float));
        string s = read();              // todo works?
        *f = stof(s);
    }
    virtual ~SocketIO(){};
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID) override{
        SocketIO sock(clientID);
        CLI c(&sock);

      /*  char buffer[1024];
        int n = read(clientID, buffer, 100);
        cout << buffer;                        */     //todo: dio?

        c.start(); //???
    }
};


// implement on Server.cpp
class Server {
    thread* t; // the thread to run the start() method in
    // you may add data members
    int fd;
    sockaddr_in server;
    sockaddr_in client;
public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};


#endif //MILESTONE3_SERVER_H
