#include "PingRequest.hpp"

PingRequest::PingRequest(handle_t socket) : Request(socket) {}

void PingRequest::AcceptValidator(Validator *validator) {}

void PingRequest::AcceptCommand(Command *command) {}

void PingRequest::SetToken(const std::string &token) {
    mToken = token;
}

const std::string &PingRequest::GetToken() const {
    return mToken;
}