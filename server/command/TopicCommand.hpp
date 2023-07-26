#pragma once

#include "Command.hpp"
#include "TopicRequest.hpp"

class TopicCommand : public Command {

public:
    void Execute() const;

    void setTopicRequest(TopicRequest *topicRequest);

private:
    TopicRequest *mTopicRequest;

};