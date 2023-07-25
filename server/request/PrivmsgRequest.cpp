#include "PrivmsgRequest.hpp"

PrivmsgRequest::PrivmsgRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void PrivmsgRequest::AddTarget(const std::string &target) {
    mTargets.push_back(target);
}

void PrivmsgRequest::SetMessage(const std::string &message) {
    mMessage = message;
}

const std::vector<std::string> &PrivmsgRequest::GetTargets() const {
    return mTargets;
}

const std::string &PrivmsgRequest::GetMessage() const {
    return mMessage;
}