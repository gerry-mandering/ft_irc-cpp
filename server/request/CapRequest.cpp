#include "CapRequest.hpp"

CapRequest::CapRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void CapRequest::SetSubCommand(const std::string &subCommand) {
    mSubCommand = subCommand;
}

const std::string &CapRequest::GetSubCommand() const {
    return mSubCommand;
}