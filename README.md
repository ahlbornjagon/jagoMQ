![](image/jagoMQ_whatdadogdoin.png)

# jagoMQ

A lightweight C++ message queue library providing publisher-subscriber messaging patterns with TCP transport. I have been using services such as RabbitMQ and ZMQ a lot lately, so I wanted to write my own basic version to understand the inner-workings.

## Features

- **Publisher-Subscriber Pattern**: Simple pub/sub messaging architecture *TODO:* Right now it just supports basic sending and rec. Need to add pattern support 
- **TCP Transport**: Reliable network communication using TCP sockets
- **Thread-Safe**: Multi-threaded support for concurrent operations *TODO:* Need to add mutex's
- **Header-Only Interface**: Easy integration with single header include
- **Cross-Platform**: Compatible with Linux and other Unix-like systems

## Requirements

- C++17 or later
- pthread support
- Standard Unix socket libraries

## Building

### Build the Library

```bash
make
```

This creates `libjagoMQ.a` static library.

### Clean Build Files

```bash
make clean
```

## Usage

### Basic Publisher Example

```cpp
#include "jagoMQ.hpp"
#include <iostream>

int main() {
    Publisher pub;
    
    if (!pub.bind("0.0.0.0", 8082)) {
        std::cerr << "Failed to bind publisher" << std::endl;
        return 1;
    }
    
    if (!pub.start()) {
        std::cerr << "Failed to start publisher" << std::endl;
        return 1;
    }
    
    std::vector<std::string> subscriber_ips = pub.getSubscriberIPs();

    for (const auto& client_ip : subscriber_ips) {
        pub.sendMessage(message, client_ip);
        fprintf(stdout, "Sent message to %s\n", client_ip.c_str());
    
    return 0;
}
```

### Basic Subscriber Example

```cpp
#include "jagoMQ.hpp"
#include <iostream>

void messageCallback(const std::string& message) {
    std::cout << "Received: " << message << std::endl;
}

int main() {
    Subscriber sub;
    
    if (!sub.connect("127.0.0.1", 8082)) {
        std::cerr << "Failed to connect subscriber" << std::endl;
        return 1;
    }
    
    sub.setCallback(messageCallback);
    sub.start();
    
    // Keep running to receive messages
    std::this_thread::sleep_for(std::chrono::seconds(30));
    
    return 0;
}
```

## Examples

The `examples/` directory contains complete working examples:

- `demo_publisher.cpp`: Publisher implementation
- `demo_subscriber.cpp`: Subscriber implementation

### Building Examples

```bash
cd examples
make
```

### Running Examples

Terminal 1 (Publisher):
```bash
./demo_publisher
```

Terminal 2 (Subscriber):
```bash
./demo_subscriber 127.0.0.1 8082
```

## API Reference

### Publisher Class

- `bool bind(const std::string& address, int port)`: Bind to address and port
- `bool start()`: Start the publisher service
- `void sendMessage(const std::string& message, const std::string& clientIP)`: Send message to a clientIP.
- `void stop()`: Stop the publisher service
- *TODO:* Need to add publish method that just sends to all ip's so user doesnt have to write it. Logic exists in publisher example

### Subscriber Class

- `bool connect(const std::string& address, int port)`: Connect to publisher
- `void setCallback(std::function<void(const std::string&)> callback)`: Set message callback
- `bool start()`: Start receiving messages
- `void stop()`: Stop the subscriber

## Architecture

jagoMQ uses a simple client-server architecture where:

- **Publishers** act as servers, binding to a port and accepting connections
- **Subscribers** act as clients, connecting to publishers
- **TCP_Transport** is a layer that abstracts all underlying socket logic from the user facing publisher and subscriber functions.
- Messages are delivered to all connected subscribers
- TCP sockets provide reliable, ordered message delivery

## Acknowledgments

Endless riches to [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) for socket programming guidance. I wanted to keep AI help to a minumum and this guide provided me with 95% of the reference I needed to complete this project.