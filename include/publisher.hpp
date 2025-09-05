#pragma once
#include <string>    
#include "transport.hpp"
#include <vector>


class Publisher {
public: 
    bool bind(const std::string& address, int port);
    bool start();
    void stop();

    void sendMessage(const std::string& message, const std::string& clientIP);
    int broadcast(const std::string& message);
    int subscriber_count() const;
    std::vector<std::string> getSubscriberIPs() const;
    bool running() const;

private:
    TcpTransport transport_;
    std::vector<std::string> subscribers_;
    bool running_=false;
};