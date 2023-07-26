#pragma once

#include "Request.hpp"

class TopicRequest : public Request {

public:
    TopicRequest(handle_t socket);

    void AcceptValidator(Validator *validator);
    void AcceptCommand(Command *command);

    void SetChannelName(const std::string &channelName);
    void SetTopic(const std::string &topic);

    const std::string &GetChannelName() const;
    const std::string &GetTopic() const;

private:
    std::string mChannelName;
    std::string mTopic;

};