#include "CapRequest.hpp"

CapRequest::CapRequest(handle_t socket, const std::string &subCommand) : Request(socket), mSubCommand(subCommand) {}

bool CapRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void CapRequest::SetSubCommand(const std::string &subCommand)
{
    mSubCommand = subCommand;
}

const std::string &CapRequest::GetSubCommand() const
{
    return mSubCommand;
}
