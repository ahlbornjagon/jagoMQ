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
#include <stdexcept>


//Eventually this can be dynamic, first sent message includes length, next includes message
#define MAX_RECV_BUF_SIZE 4096

struct ClientInfo{
    int fd;
    std::string ip;
};


class TcpTransport{
public:
    // Publisher Definitions
    bool bind(const std::string& address, int port);
    bool start();
    void stop();
    int send(const std::string& message, const std::string& ip);
    int broadcast(const std::string& message);
    int subscriber_count() const;
    std::vector<std::string> getSubscriberIPs() const;
    bool running() const;
    void acceptThread();

    // Subscriber Definitions
    bool connect(std::string& address, int port);
    std::string getMsg();
    void close();

private:
    int socketfd_ = -1;
    bool isBinded_ = false;
    bool isListening_ = false;
    std::vector<ClientInfo> clients_;
    std::thread accept_thread_;
    std::vector<char> recv_buffer_;
};