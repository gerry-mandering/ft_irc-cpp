#include "NickRequest.hpp"

NickRequest::NickRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void NickRequest::SetNickName(const std::string &nickName) {
    mNickname = nickName;
}

const std::string &NickRequest::GetNickName() const {
    return mNickname;
}