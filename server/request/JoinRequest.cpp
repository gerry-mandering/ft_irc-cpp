#include "JoinRequest.hpp"

JoinRequest::JoinRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void JoinRequest::SetChannelName(const std::string &channelName) {
    mChannelNames.push_back(channelName);
}

void JoinRequest::SetKey(const std::string &key) {
    mKeys.push_back(key);
}

const std::vector <std::string> &JoinRequest::GetChannelNames() const {
    return mChannelNames;
}

const std::string &JoinRequest::GetKeys() const {
    return mKeys;
}