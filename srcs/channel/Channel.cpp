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

bool Channel::CheckClientIsExist(const std::string &nickName)
{
    std::vector<Client *>::iterator iter = mClients.begin();

    while (iter != mClients.end())
    {
        if ((*iter)->GetNickName() == nickName)
            return true;

        iter++;
    }

    return false;
}

bool Channel::CheckClientIsOperator(const std::string &nickName)
{
    std::vector<Client *>::iterator iter = mOperators.begin();

    while (iter != mOperators.end())
    {
        if ((*iter)->GetNickName() == nickName)
            return true;

        iter++;
    }

    return false;
}

void Channel::SetClient(Client *newClient)
{
    mClients.push_back(newClient);
}

void Channel::RemoveClient(const std::string &nickName)
{
    if (CheckClientIsOperator(nickName))
        RemoveOperator(nickName);

    std::vector<Client *>::iterator iter;

    iter = mClients.begin();
    while (iter != mClients.end())
    {
        if ((*iter)->GetNickName() == nickName)
        {
            mClients.erase(iter);
            break;
        }

        iter++;
    }
}

void Channel::SetOperator(Client *newOperator)
{
    mOperators.push_back(newOperator);
}

void Channel::RemoveOperator(const std::string &nickName)
{
    std::vector<Client *>::iterator iter;

    iter = mOperators.begin();
    while (iter != mOperators.end())
    {
        if ((*iter)->GetNickName() == nickName)
        {
            mOperators.erase(iter);
            break;
        }

        iter++;
    }
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
