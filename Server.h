// 316482355 Oz Browner

//compile: g++ -pthread -std=c++11 anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h timeseries.h SimpleAnomalyDetector.h timeseries.cpp SimpleAnomalyDetector.cpp HybridAnomalyDetector.h HybridAnomalyDetector.cpp CLI.cpp CLI.h minCircle.cpp minCircle.h commands.h StandardIO.cpp StandardIO.h MainTrain.cpp Server.cpp Server.h

#ifndef MILESTONE3_SERVER_H
#define MILESTONE3_SERVER_H

#include "CLI.h"
#include <iostream>
#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

#include <unistd.h>

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

    /**
     * gets a string represent float and cut zeros at its end.
     * @param s
     * @return
     */
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
        recv(fd,&c,1,0);
        while (c != '\n') {
            s += c;
            recv(fd,&c,1,0);
        }
        return s;
    }
    virtual void write(string text) {
        send(fd, text.c_str(), text.length(), 0);
    }

    virtual void write(float f) {
        string s = to_string(f);
        s = strFloatCut(s);
        write(s);
    }
    virtual void read(float* f) {
        string s = read();
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
        c.start();
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
