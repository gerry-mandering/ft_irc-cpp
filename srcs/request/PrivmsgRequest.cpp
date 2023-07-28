#include "PrivmsgRequest.hpp"

PrivmsgRequest::PrivmsgRequest(handle_t socket, const std::vector<std::string> &targets, const std::string &message)
    : Request(socket), mTargets(targets), mMessage(message)
{
}

void PrivmsgRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void PrivmsgRequest::AddTarget(const std::string &target)
{
    mTargets.push_back(target);
}

void PrivmsgRequest::SetMessage(const std::string &message)
{
    mMessage = message;
}

std::vector<std::string> PrivmsgRequest::GetTargets() const
{
    return mTargets;
}

const std::string &PrivmsgRequest::GetMessage() const
{
    return mMessage;
}
