#pragma once
#include <std.h>

class Publisher {
public: 
    bool bind(const std::string& address, int port);
    void start();
    void stop();

    void sendMessage(const std::string& message);
    int subscriber_count();

    bool running();

private:
    TcpTransport transport_;
    std::vector<int> subscribers_;
    bool running_=false;
};