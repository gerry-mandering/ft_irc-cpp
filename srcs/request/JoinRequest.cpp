#include "JoinRequest.hpp"

JoinRequest::JoinRequest(handle_t socket, const std::string &channelName, const std::string &key)
    : Request(socket), mChannelName(channelName), mKey(key)
{
    LOG_TRACE("JoinRequest constructor called | " << *this);
}

bool JoinRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void JoinRequest::setChannelName(const std::string &channelName)
{
    mChannelName = channelName;
}

void JoinRequest::setKey(const std::string &key)
{
    mKey = key;
}

const std::string &JoinRequest::getChannelName() const
{
    return mChannelName;
}

const std::string &JoinRequest::getKey() const
{
    return mKey;
}

std::stringstream &operator<<(std::stringstream &ss, const JoinRequest &joinRequest)
{
    ss << "JoinRequest = { ChannelName: " << joinRequest.mChannelName << ", Key: " << joinRequest.mKey << " }";

    return ss;
}
