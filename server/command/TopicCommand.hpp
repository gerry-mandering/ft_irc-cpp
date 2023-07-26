#pragma once

#include "Command.hpp"
#include "TopicRequest.hpp"

class TopicCommand : public Command {

public:
    void Execute(TopicRequest *topicRequest) const;

};