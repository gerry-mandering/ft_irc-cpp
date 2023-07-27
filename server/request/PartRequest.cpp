#include "PartRequest.hpp"

PartRequest::PartRequest(handle_t socket) : Request(socket) {}

void PartRequest::AcceptValidator(Validator *validator) {}

void PartRequest::AcceptCommand(Command *command) {}

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