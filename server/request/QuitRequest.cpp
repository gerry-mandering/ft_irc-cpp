#include "QuitRequest.hpp"

QuitRequest::QuitRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void QuitRequest::SetReason(const std::string &reason) {
    mReason = reason;
}

const std::string &QuitRequest::GetReason() const {
    return mReason;
}