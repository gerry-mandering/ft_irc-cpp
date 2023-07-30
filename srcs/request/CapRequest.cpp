#include "CapRequest.hpp"

CapRequest::CapRequest(handle_t socket, const std::string &subCommand) : Request(socket), mSubCommand(subCommand)
{
    LOG_TRACE("CapRequest constructor: " << *this);
}

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

std::ostream &operator<<(std::ostream &os, const CapRequest &capRequest)
{
    os << "CapRequest - SubCommand: " << capRequest.mSubCommand;

    return os;
}
