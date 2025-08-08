#include "transport.hpp"

bool TcpTransport::bind(const std::string& address, int port)
{
    struct addrinfo hints, *res;
    struct addrinfo *servinfo;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    const char* portChar = portStr.c_str();
    const char* addrChar = address.c_str();

    if(int status = getaddrinfo(addrChar, portChar, &hints, &servinfo) != 0){
        fprintf(stderr, "address info error:  %s\n",gai_strerror(status) );
        return;
    };

    socketfd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(::bind(socketfd_, res->ai_addr, res->ai_addrlen) != 0){
        fprintf(stderr, "Error on bind: %s\n", strerror(errno));
        return;
    };

    isBinded_ = true;
    freeaddrinfo(res);
}

bool TcpTransport::start()
{
    if(socketfd_ == -1 || isBinded_ == false){

        fprintf(stderr, "Please bind on a socket before calling listen");
        return;
    };

    if (int status = ::listen(socketfd_, 10)  != 0){        // Hardcoding backlog = 10 queued connection requests
        fprintf(stderr, "Bad listener smh, check this out: %s\n", strerror(errno));
        return;
    };

    isListening_ = true;

    std::thread accept_thread (void accept_thread_());

    

    return true;
}

void TcpTransport::acceptThread()
{
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;
    
    while (isBinded_ && isListening_)
    {
        int new_fd = accept(socketfd_, (struct sockaddr *)&client_addr, &addr_size);
        
        if (new_fd != -1){
            clients_.push_back(new_fd);
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

void TcpTransport::send(const std::string& message)
{
    if ()
}


