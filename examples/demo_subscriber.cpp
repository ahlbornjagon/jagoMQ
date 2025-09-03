#include <string>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <sys/select.h>
#include <unistd.h>
#include "../include/jagoMQ.hpp"

struct hostConfig {
    std::string ip;
    int port;
};

void cb(const std::string& msg);
hostConfig parseArgs(int argc, char* argv[]);


int main(int argc, char* argv[])
{

    hostConfig config = parseArgs(argc, argv);
    Subscriber sub;
    if(!sub.connect(config.ip, config.port)){
        std::cerr << "Could not connect to publisher at " << config.ip << ":" << config.port << "\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to publisher at " << config.ip << ":" << config.port << "\n";
    std::cout << "Setting callback...\n";
    sub.set_callback(&cb);
    std::cout << "Starting receive loop...\n";
    sub.start_recv_loop();
    
    std::cout << "Press Enter to exit, or wait for publisher to disconnect...\n";
    
    // Allows the program to exit if connection is lost or user intervention
    // TODO: I think user intervention is broke :( 
    while (sub.is_connected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval timeout = {0, 0};
        
        if (select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &timeout) > 0) {
            std::string input;
            std::getline(std::cin, input);
            break;
        }
    }
    
    if (!sub.is_connected()) {
        std::cout << "Connection lost. Exiting...\n";
    }
    
    sub.stop_recv_loop();
    sub.close();
}

void cb(const std::string& msg){
    std::cout << "Message received!\n";
    std::cout << msg << "\n";

}

hostConfig parseArgs(int argc, char* argv[])
{

    if (argc != 3){
        std::cerr << "Usage: demo_subscriber <IP_ADDRESS> <PORT>\n";
        exit(EXIT_FAILURE);
    }
    hostConfig config;
    config.ip = argv[1];
    config.port = std::stoi(argv[2]);
    return config;
    
}