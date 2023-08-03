#include "PrivmsgRequest.hpp"

PrivmsgRequest::PrivmsgRequest(handle_t socket, const std::vector< std::string > &targets, const std::string &message)
    : Request(socket), mTargets(targets), mMessage(message)
{
    LOG_TRACE("UserRequest constructor called | " << *this);
}

bool PrivmsgRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void PrivmsgRequest::AddTarget(const std::string &target)
{
    mTargets.push_back(target);
}

void PrivmsgRequest::SetMessage(const std::string &message)
{
    mMessage = message;
}

std::vector< std::string > &PrivmsgRequest::GetTargets()
{
    return mTargets;
}

const std::string &PrivmsgRequest::GetMessage() const
{
    return mMessage;
}

std::stringstream &operator<<(std::stringstream &ss, PrivmsgRequest &privmsgRequest)
{
    ss << "PrivmsgRequest = { Targets: ";

    std::vector< std::string >::iterator iter;
    for (iter = privmsgRequest.mTargets.begin(); iter != privmsgRequest.mTargets.end(); iter++)
        ss << *iter << ", ";

    ss << "Message: " << privmsgRequest.mMessage << " }";

    return ss;
}
