#include "ConnectionInitializer.hpp"

const std::string ConnectionInitializer::PORT_NUMBER = "6667";

ConnectionInitializer::ConnectionInitializer() {
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    try {
        wrapper::wGetaddrinfo(NULL, PORT_NUMBER.c_str(), &hints, &mAddressInfo);
        mListeningSocket = wrapper::wSocket(mAddressInfo->ai_family, mAddressInfo->ai_socktype,
                                            mAddressInfo->ai_protocol);
        wrapper::wBind(mListeningSocket, mAddressInfo->ai_addr, mAddressInfo->ai_addrlen);
        freeaddrinfo(mAddressInfo);
        wrapper::wListen(mListeningSocket, BACKLOG_QUEUE_SIZE);
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

ConnectionInitializer::~ConnectionInitializer() {}

void ConnectionInitializer::HandleRead() {
    reactor::Socket clientSocket = accept(mListeningSocket, NULL, NULL);
    if (clientSocket == -1) {
        std::cerr << "Fail to accept client connection" << std::endl;
        return;
    }

    fcntl(clientSocket, F_SETFL, O_NONBLOCK);

    reactor::EventHandler *requestHandler = new RequestHandler(clientSocket);
    Reactor::GetInstance()->RegisterHandler(requestHandler, reactor::EVENT_READ);
}

reactor::Socket ConnectionInitializer::GetSocket() const {
    return mListeningSocket;
}
