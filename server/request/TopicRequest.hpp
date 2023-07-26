#pragma once

#include "Request.hpp"
#include "TopicCommand.hpp"

class TopicRequest : public Request {

public:
    TopicRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetChannelName(const std::string &channelName);
    void SetTopic(const std::string &topic);

    const std::string &GetChannelName() const;
    const std::string &GetTopic() const;

private:
    std::string mChannelName;
    std::string mTopic;

};