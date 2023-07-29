#pragma once

#include "Request.hpp"

class TopicRequest : public Request
{

  public:
    TopicRequest(handle_t socket, const std::string &channelName, const std::string &topic);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetChannelName(const std::string &channelName);
    void SetTopic(const std::string &topic);

    const std::string &GetChannelName() const;
    const std::string &GetTopic() const;

  private:
    std::string mChannelName;
    std::string mTopic;
};