#include "PingRequest.hpp"

PingRequest::PingRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void PingRequest::SetToken(const std::string &token) {
    mToken = token;
}

const std::string &PingRequest::GetToken() const {
    return mToken;
}