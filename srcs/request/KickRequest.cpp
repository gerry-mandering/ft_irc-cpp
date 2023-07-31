#include "KickRequest.hpp"

KickRequest::KickRequest(handle_t socket, const std::string &channelName, const std::vector<std::string> &nickNames,
                         const std::string &message)
    : Request(socket), mChannelName(channelName), mTargets(nickNames), mMessage(message)
{
    LOG_TRACE("KickRequest constructor called | " << *this);
}

bool KickRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void KickRequest::SetChannelName(const std::string &channelName)
{
    mChannelName = channelName;
}

void KickRequest::SetTarget(const std::string &target)
{
    mTargets.push_back(target);
}

void KickRequest::SetMessage(const std::string &message)
{
    mMessage = message;
}

const std::string &KickRequest::GetChannelName() const
{
    return mChannelName;
}

std::vector<std::string> &KickRequest::GetTargets()
{
    return mTargets;
}

const std::string &KickRequest::GetMessage() const
{
    return mMessage;
}

// TODO const 아니여도 ㄱㅊ?
std::ostream &operator<<(std::ostream &os, KickRequest &kickRequest)
{
    os << "KickRequest = { ChannelName: " << kickRequest.mChannelName << ", NickNames: ";

    std::vector<std::string>::iterator iter;
    for (iter = kickRequest.mTargets.begin(); iter != kickRequest.mTargets.end(); iter++)
        os << *iter << ", ";

    os << "Message: " << kickRequest.mMessage << " }";

    return os;
}
