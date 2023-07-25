#include "PartRequest.hpp"

PartRequest::PartRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void PartRequest::SetChannelName(const std::string &channelName) {
    mChannelName = channelName;
}

void PartRequest::SetReason(const std::string &reason) {
    mReason = reason;
}

const std::string &PartRequest::GetChannelName() const {
    return mChannelName;
}

const std::string &PartRequest::GetReason() const {
    return mReason;
}