#include "PrivmsgRequest.hpp"

PrivmsgRequest::PrivmsgRequest(handle_t socket) : Request(socket)
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

const std::vector<std::string> &PrivmsgRequest::GetTargets() const
{
    return mTargets;
}

const std::string &PrivmsgRequest::GetMessage() const
{
    return mMessage;
}