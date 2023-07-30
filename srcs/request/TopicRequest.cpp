#include "TopicRequest.hpp"

TopicRequest::TopicRequest(handle_t socket, const std::string &channelName, const std::string &topic)
    : Request(socket), mChannelName(channelName), mTopic(topic)
{
    LOG_TRACE("TopicRequest constructor called | " << *this);
}

bool TopicRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void TopicRequest::SetChannelName(const std::string &channelName)
{
    mChannelName = channelName;
}

void TopicRequest::SetTopic(const std::string &topic)
{
    mTopic = topic;
}

const std::string &TopicRequest::GetChannelName() const
{
    return mChannelName;
}

const std::string &TopicRequest::GetTopic() const
{
    return mTopic;
}

std::ostream &operator<<(std::ostream &os, const TopicRequest &topicRequest)
{
    os << "TopicRequest = { ChannelName: " << topicRequest.mChannelName << ", Topic: " << topicRequest.mTopic << " }";

    return os;
}
