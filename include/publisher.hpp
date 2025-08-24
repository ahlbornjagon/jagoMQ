#pragma once
#include <string>    
#include "transport.hpp"
#include <vector>


class Publisher {
public: 
    bool bind(const std::string& address, int port);
    void start();
    void stop();

    void sendMessage(const std::string& message, const std::string& clientIP);
    int subscriber_count() const;

    bool running() const;

private:
    TcpTransport transport_;
    std::vector<int> subscribers_;
    bool running_=false;
};