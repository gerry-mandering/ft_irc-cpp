#include "Channel.hpp"

Channel::Channel(const std::string &name)
    : mName(name), mTopic(std::string()), mbInviteOnlyMode(false), mbProtectedTopicMode(false), mbKeyMode(false),
      mbClientLimitMode(false)
{
}

void Channel::BroadcastMessage(const std::string &message)
{
    std::vector<Client *>::iterator iter = mClients.begin();

    while (iter != mClients.end())
    {
        (*iter)->InsertResponse(message);
        iter++;
    }
}

void Channel::SetClient(Client *newClient)
{
    mClients.push_back(newClient);
}

const std::string &Channel::GetTopic() const
{
    return mTopic;
}

void Channel::SetTopic(const std::string &topic)
{
    mTopic = topic;
}

bool Channel::IsInviteOnlyMode() const
{
    return mbInviteOnlyMode;
}

bool Channel::IsProtectedTopicMode() const
{
    return mbProtectedTopicMode;
}

bool Channel::IsKeyMode() const
{
    return mbKeyMode;
}

bool Channel::IsClientLimitMode() const
{
    return mbClientLimitMode;
}

void Channel::ToggleInviteOnlyMode()
{
    mbInviteOnlyMode ^= true;
}

void Channel::ToggleProtectedTopicMode()
{
    mbProtectedTopicMode ^= true;
}

void Channel::ToggleKeyMode()
{
    mbKeyMode ^= true;
}

void Channel::ToggleClientLimitMode()
{
    mbClientLimitMode ^= true;
}
