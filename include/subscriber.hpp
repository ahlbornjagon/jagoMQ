#pragma once
#include <string>       
#include <functional>   
#include <thread>      
#include <stdexcept>
#include "transport.hpp"

class Subscriber{
public:
    bool connect(std::string& address, int port);
    std::string recv() const; // This is a single callable function to receive data
    void start_recv_loop(); // This will kick off recv thread that listens and will call callback function
    void set_callback(std::function<void(const std::string&)> callback); // You must set the callback function before this...or else I cry
    void stop_recv_loop();
    void close();
    

private:
    TcpTransport transport_;
    bool loop_running_ = false;
    std::thread listener_thread_;
    std::function<void(const std::string&)> callback_;
    void recv_loop();
    



};