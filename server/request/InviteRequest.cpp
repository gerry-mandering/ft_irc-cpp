#include "InviteRequest.hpp"

InviteRequest::InviteRequest(handle_t socket) : Request(socket)
{
}

void InviteRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void InviteRequest::SetNickName(const std::string &nickName)
{
    mNickName = nickName;
}

void InviteRequest::SetChannelName(const std::string &channelName)
{
    mChannelName = channelName;
}

const std::string &InviteRequest::GetNickName() const
{
    return mNickName;
}

const std::string &InviteRequest::GetChannelName() const
{
    return mChannelName;
}