#include "Server.hpp"

const std::string Server::PORT_NUMBER = "6667";

Server::Server() {}

Server::~Server() {}

Server::Server(const Server &other) {}

Server &Server::operator=(const Server &server) {}

// 이부분 맘에 안듬
void Server::Init() {
    initConnectionRequestSocket();
    initServerAddress();
    if (bind(mConnectionRequestSocket, mServerAddress->ai_addr, mServerAddress->ai_addrlen) == -1) {
        std::cerr << "Fail to bind socket" << std::endl;
        return;
    }
    if (listen(mConnectionRequestSocket, Server::BACKLOG_QUEUE_SIZE) == -1) {
        std::cerr << "Fail to listen for connections" << std::endl;
        return;
    }
}

reactor::Socket Server::GetSocket() const {
    return mConnectionRequestSocket;
}

void Server::HandleRead() {
    reactor::Socket newConnectionSocket = accept(mConnectionRequestSocket, NULL, NULL);
    if (newConnectionSocket == -1) {
        std::cerr << "Fail to accept client connection" << std::endl;
        return;
    }

    RequestHandler *requestHandler = new RequestHandler(newConnectionSocket);

    Reactor *reactor = Reactor::GetInstance();
    reactor.RegisterHandler(requestHandler, reactor::READ);
}

void Server::initServerAddress() {
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(NULL, Server::PORT_NUMBER.c_str(), &hints, &mServerAddress);
    if (result != 0) {
        std::cerr << "Failed to initialize server address: " << gai_strerror(result) << std::endl;
        return;
    }
}

void Server::initConnectionRequestSocket() {
    mConnectionRequestSocket = socket(mServerAddress->ai_family,
                                      mServerAddress->ai_socktype,
                                      mServerAddress->ai_protocol);
    if (mConnectionRequestSocket == -1) {
        std::err << "Fail to initialize connection request socket" << std::endl;
        return;
    }
}