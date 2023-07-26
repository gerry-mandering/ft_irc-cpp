#include "TopicCommand.hpp"

void TopicCommand::Execute() const {}

void TopicCommand::setTopicRequest(TopicRequest *topicRequest) {
    mTopicRequest = topicRequest;
}
