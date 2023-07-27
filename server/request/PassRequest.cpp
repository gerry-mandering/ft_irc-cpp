#include "PassRequest.hpp"

PassRequest::PassRequest(handle_t socket) : Request(socket) {}

void PassRequest::AcceptValidator(Validator *validator) {}

void PassRequest::AcceptCommand(Command *command) {}

void PassRequest::SetPassword(const std::string &password) {
    mPassword = password;
}

const std::string &PassRequest::GetPassword() const {
    return mPassword;
}