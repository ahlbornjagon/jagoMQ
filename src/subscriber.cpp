#include "subscriber.hpp"
#include <iostream>

bool Subscriber::connect(std::string &address, int port){

    return transport_.connect(address, port);
}

std::string Subscriber::recv() {

    return transport_.getMsg();
}

void Subscriber::start_recv_loop(){

    if (!callback_){
        throw std::runtime_error("Now I cry, pls set the callback function first with 'set_callback(urFunc)'");
    };

    loop_running_ = true;
    Subscriber::listener_thread_ = std::thread(&Subscriber::recv_loop, this);
}

void Subscriber::recv_loop(){

    while(loop_running_){
        try {
        std::string msg = recv();
        if (msg == "") {
            continue;
        }
        callback_(msg);
        } catch (const std::exception& e){
            std::cerr << "Error in receive loop: " << e.what() << '\n';
            if (std::string(e.what()).find("Connection closed") != std::string::npos) {
                std::cout << "Publisher disconnected, stopping receive loop\n";
                loop_running_ = false;
                connection_active_ = false;
                break;
            }
        }
    }
}

void Subscriber::stop_recv_loop(){

    loop_running_ = false;
    if (listener_thread_.joinable()) {
    listener_thread_.join();
    }

}

void Subscriber::set_callback(std::function<void(const std::string&)> callback){
    fprintf(stdout, "Setting callback function...\n");
    callback_ = callback;
}

void Subscriber::close(){
    transport_.close();
}

bool Subscriber::is_connected() const {
    return connection_active_;
}


