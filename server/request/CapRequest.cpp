#include "CapRequest.hpp"

CapRequest::CapRequest(handle_t socket) : Request(socket)
{
}

void CapRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void CapRequest::SetSubCommand(const std::string &subCommand)
{
    mSubCommand = subCommand;
}

const std::string &CapRequest::GetSubCommand() const
{
    return mSubCommand;
}