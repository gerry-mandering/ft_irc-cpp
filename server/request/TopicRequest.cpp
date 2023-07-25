#include "TopicRequest.hpp"

TopicRequest::TopicRequest(handle_t socket) : Request(socket) {}

void TopicRequest::ValidateRequest() const {

}

Command *TopicRequest::BuildCommand() const {
    TopicCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void TopicRequest::SetChannelName(const std::string &channelName) {
    mChannelName = channelName;
}

void TopicRequest::SetTopic(const std::string &topic) {
    mTopic = topic;
}

const std::string &TopicRequest::GetChannelName() const {
    return mChannelName;
}

const std::string &TopicRequest::GetTopic() const {
    return mTopic;
}
