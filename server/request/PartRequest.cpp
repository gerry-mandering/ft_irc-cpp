#include "PartRequest.hpp"

PartRequest::PartRequest(handle_t socket, const std::string &channelName, const std::string &reason)
    : Request(socket), mChannelName(channelName), mReason(reason)
{
}

void PartRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void PartRequest::SetChannelName(const std::string &channelName)
{
    mChannelName = channelName;
}

void PartRequest::SetReason(const std::string &reason)
{
    mReason = reason;
}

const std::string &PartRequest::GetChannelName() const
{
    return mChannelName;
}

const std::string &PartRequest::GetReason() const
{
    return mReason;
}
