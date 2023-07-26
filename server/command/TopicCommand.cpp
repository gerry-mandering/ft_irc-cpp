#include "TopicCommand.hpp"

void TopicCommand::Execute() const {}

void TopicCommand::SetTopicRequest(TopicRequest *topicRequest) {
    mTopicRequest = topicRequest;
}
