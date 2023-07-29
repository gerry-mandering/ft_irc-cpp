#include "KickRequest.hpp"

KickRequest::KickRequest(handle_t socket, const std::vector<std::string> &nickNames, const std::string &message)
    : Request(socket), mNickNames(nickNames), mMessage(message)
{
}

bool KickRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void KickRequest::SetNickName(const std::string &nickName)
{
    mNickNames.push_back(nickName);
}

void KickRequest::SetMessage(const std::string &message)
{
    mMessage = message;
}

std::vector<std::string> &KickRequest::GetNickNames()
{
    return mNickNames;
}

const std::string &KickRequest::GetMessage() const
{
    return mMessage;
}
