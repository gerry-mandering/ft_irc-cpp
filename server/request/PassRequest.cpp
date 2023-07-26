#include "PassRequest.hpp"

PassRequest::PassRequest(handle_t socket) : Request(socket) {}

void PassRequest::SetPassword(const std::string &password) {
    mPassword = password;
}

const std::string &PassRequest::GetPassword() const {
    return mPassword;
}