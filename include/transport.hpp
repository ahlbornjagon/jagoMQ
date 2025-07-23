#pragma once
#include <string>

class TcpTransport{
public:
    // Publisher Definitions
    bool bind(const std::string& address, int port);
    void start();
    void stop();
    void send(const std::string& message);
    int subscriber_count() const;
    bool running() const;

    // Subscriber Definitions

private:
    //TODO: Add all private classes
}