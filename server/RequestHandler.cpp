#include "RequestHandler.hpp"

RequestHandler::RequestHandler() {}

RequestHandler::~RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler &other) {}

RequestHandler &RequestHandler::operator=(const RequestHandler &other) {}

RequestHandler::RequestHandler(const reactor::Socket clientSocket) : mClientSocket(clientSocket) {}

reactor::Socket RequestHandler::GetSocket() const {
    return mClientSocket;
}

void RequestHandler::HandleRead() {
    char buffer[1024];

    //Kernel buffer size 수정?
    int length = recv(mClientSocket, buffer, 1024, 0);
    std::cout << buffer << std::endl;
}

void RequestHandler::HandleWrite() {

}

void RequestHandler::HandleError() {

}

