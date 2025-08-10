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


struct ClientInfo{
    int fd;
    std::string ip;
}


class TcpTransport{
public:
    // Publisher Definitions
    bool bind(const std::string& address, int port);
    bool start();
    void stop();
    int send(const std::string& message, const std::string& ip);
    void broadcast(const std::string$ message);
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
    std::vector<ClientInfo> clients_;
    void acceptThread_();
    std::thread accept_thread_;
};