#include "CapRequest.hpp"

CapRequest::CapRequest(handle_t socket) : Request(socket) {}

void CapRequest::SetSubCommand(const std::string &subCommand) {
    mSubCommand = subCommand;
}

const std::string &CapRequest::GetSubCommand() const {
    return mSubCommand;
}