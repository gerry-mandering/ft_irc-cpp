#include "KickRequest.hpp"

KickRequest::KickRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void KickRequest::SetNickName(const std::string &nickName) {
    mNickNames.push_back(nickName);
}

void KickRequest::SetMessage(const std::string &message) {
    mMessage = message;
}

const std::vector <std::string> &KickRequest::GetNickNames() const {
    return mNickNames;
}

const std::string &KickRequest::GetMessage() const {
    return mMessage;
}