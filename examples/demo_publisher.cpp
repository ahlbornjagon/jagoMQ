#include <string>
#include <iostream>
#include <cstdlib>
#include "../include/jagoMQ.hpp"

int main()
{
    Publisher pub;
    std::string address = "0.0.0.0";
    int port = 8082 ;

    if(!pub.bind(address, port)){
        std::cerr << "Failed to bind to address " << address << " on port " << port << std::endl;
        return EXIT_FAILURE;
    }
    bool pubStart = pub.start();
     if(!pubStart){
        std::cerr << "Failed to start publisher." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Publisher started on " << address << ":" << port << std::endl;
    std::cout<<"Enter messages to publish: (enter quit or exit to stop)"<<std::endl;
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message.empty()) continue;

        if (message == "quit" || message == "exit") break;

        std::vector<std::string> subscriber_ips = pub.getSubscriberIPs();
        std::cout << "Number of subscribers: " << subscriber_ips.size() << std::endl;

        pub.broadcast(message);
    }
    pub.stop();
    std::cout << "Publisher stopped." << std::endl;
    return EXIT_SUCCESS;
}