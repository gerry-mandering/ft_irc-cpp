#pragma once

#include "Request.hpp"

class TopicRequest : public Request {

private:
    std::string mChannelName;
    std::string mTopic;

};