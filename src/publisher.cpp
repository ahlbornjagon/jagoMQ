#include "publisher.hpp"

bool Publisher::bind(const std::string& address, int port){

    return transport_.bind(address, port) == 0;
}

void Publisher::start(){

    transport_.start();
}

void Publisher::stop(){

    transport_.stop();
}

void Publisher::sendMessage(const std::string& message){

    transport_.send(message);
}

int Publisher::subscriber_count() const{

    return transport_.subscriber_count();
}

bool Publisher::running() const{

    return running_;
}