#include "CapRequest.hpp"

CapRequest::CapRequest(handle_t socket, const std::string &subCommand) : Request(socket), mSubCommand(subCommand)
{
    LOG_TRACE("CapRequest constructor called | " << *this);
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

std::stringstream &operator<<(std::stringstream &ss, const CapRequest &capRequest)
{
    ss << "CapRequest = { SubCommand: " << capRequest.mSubCommand << " }";

    return ss;
}
