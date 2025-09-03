#include "publisher.hpp"

bool Publisher::bind(const std::string& address, int port){

    return transport_.bind(address, port);
}

bool Publisher::start(){

    return transport_.start();
}

void Publisher::stop(){

    transport_.stop();
}

void Publisher::sendMessage(const std::string& message, const std::string& clientIP){

    transport_.send(message, clientIP);
}

int Publisher::subscriber_count() const{

    return transport_.subscriber_count();
}

bool Publisher::running() const{

    return running_;
}

std::vector<std::string> Publisher::getSubscriberIPs() const {
    return transport_.getSubscriberIPs();
}


int TcpTransport::subscriber_count() const{

    return clients_.size();
}