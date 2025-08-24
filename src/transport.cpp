#include "transport.hpp"

// TODO 8/10:
//      Need to write client and server close/shutdown methods. The server will also need a recv method.
//      I think the server will a close a socket of the recv/send returns -1? Need to look into shutdown as well
//      Also need subscriber transport methods 

bool TcpTransport::bind(const std::string& address, int port)
{
    struct addrinfo hints, *res;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    const char* portChar = portStr.c_str();
    const char* addrChar = address.c_str();

    if(int status = getaddrinfo(addrChar, portChar, &hints, &servinfo) != 0){
        fprintf(stderr, "address info error:  %s\n",gai_strerror(status) );
        return false;
    };

    socketfd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(::bind(socketfd_, res->ai_addr, res->ai_addrlen) != 0){
        fprintf(stderr, "Error on bind: %s\n", strerror(errno));
        return false;
    };

    isBinded_ = true;
    freeaddrinfo(res);
    return true;
}

bool TcpTransport::start()
{
    if(socketfd_ == -1 || isBinded_ == false){

        fprintf(stderr, "Please bind on a socket before calling listen");
        return false;
    };

    if (int status = ::listen(socketfd_, 10)  != 0){        // Hardcoding backlog = 10 queued connection requests
        fprintf(stderr, "Bad listener smh, check this out: %s\n", strerror(errno));
        return false;
    };

    isListening_ = true;

    accept_thread_ = std::thread(&TcpTransport::acceptThread, this);

    return true;
}

void TcpTransport::acceptThread()
{
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof (client_addr);
    char clientIP[16];
    
    while (isBinded_ && isListening_)
    
    {
        // Dont want to do the struct cast outside of here, need it to be sockeaddr_storage to handle IPV4 and IPV6
        int new_fd = accept(socketfd_, (struct sockaddr *)&client_addr, &addr_size);
        if (new_fd != -1){
            ClientInfo client;
            client.fd = new_fd;

            // Get dat fd, now we check ipv and save the address in struct
            if(client_addr.ss_family == AF_INET){
                struct sockaddr_in* ipv4 = (struct sockaddr_in*)&client_addr;
                char ip_str[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &ipv4->sin_addr, ip_str, INET_ADDRSTRLEN);
                client.ip = ip_str;
            }
            else if(client_addr.ss_family == AF_INET6){
                struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)&client_addr;  // Fixed typo
                char ip_str[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &ipv6->sin6_addr, ip_str, INET6_ADDRSTRLEN);
                client.ip = ip_str; 
            }
            clients_.push_back(client);

        }
        else{
            fprintf(stderr, "Error accepting the clients connection request: %s\n", strerror(errno));
        }
        
    }
}

void TcpTransport::stop()
{
    if(!isListening_) return;

    isListening_ = false;
    ::close(socketfd_);
    socketfd_ = -1;
    isBinded_ = false;
}

int TcpTransport::send(const std::string& message, const std::string& clientIP )
{
    if (isBinded_ && isListening_)
    {
        if (clientIP.empty())
        {
            fprintf(stderr, "Need to specify an IP bruh, what am i supposed to do? Guess?\n");
            return -1;
        }
        if (clients_.empty()) {
            fprintf(stderr, "No clients connected\n");
            return -1;
        }

        bool found = false;
        int sendTo_fd;
        for (const auto& client:clients_)
        {
            if (client.ip == clientIP )
            {
                found = true;
                sendTo_fd = client.fd;
                break;
            }
        }
        if (!found)
        {
            fprintf(stderr, "That IP is not subscribed...aborting (awkward)");
            return -1;
        }

        int bytesSent = ::send(sendTo_fd, message.c_str(), message.length(), 0);
        return bytesSent;
    }
    fprintf(stderr, "Socket not ready for sending\n");
    return -1;
}

std::string TcpTransport::getMsg()
{
    int bytes_read = ::recv(socketfd_, recv_buffer_->data(), recv_buffer_->size(), 0);

    if(bytes_read <= 0){
        fprintf(stderr, "No data could be read smh");
        return "";
    }

    return std::string(recv_buffer_->data(), bytes_read);

}

bool TcpTransport::connect(std::string& address, int port)
{
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(port);
    getaddrinfo(address.c_str(), port_str.c_str(), &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1)
    {
        fprintf(stderr, "Connect is a no go chief, check ya address:port %s %d", address.c_str(), port);
        return false;
    }
}

void TcpTransport::close()
{
    ::close(socketfd_);
    socketfd_ = -1;

}

int TcpTransport::broadcast(const std::string& message)
{
    int subs_sent_to = 0;
    for(auto client: clients_ ){
        int ret = send(message, client.ip );
        if (ret <= 0){
            fprintf(stderr, "Couldnt broadcast to this address: %s", client.ip.c_str());
            continue;
        }
        else{
            subs_sent_to++;
        }
    }
    
    return subs_sent_to;
}








