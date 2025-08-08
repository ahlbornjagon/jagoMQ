#pragma once
#include <cstring>    
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>


class TcpTransport{
public:
    // Publisher Definitions
    bool bind(const std::string& address, int port);
    bool start();
    void stop();
    void send(const std::string& message);
    int subscriber_count() const;
    bool running() const;

    // Subscriber Definitions
    bool connect(std::string& address, int port);
    std::string getMsg() const;
    void close();



private:
    //TODO: Add all private stuff
    int socketfd_ = -1;
    bool isBinded_ = false;
    bool isListening_ = false;
    std::vector<int> clients_;
    void acceptThread();
    std::thread accept_thread_;
};