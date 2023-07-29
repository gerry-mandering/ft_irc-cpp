#include "Client.hpp"

Client::Client(handle_t socket)
    : mSocket(socket), mChannel(NULL), mbRegistered(false), mbPasswordEntered(false), mbNickNameEntered(false),
      mbUserInfoEntered(false)
{
}

void Client::InsertResponse(const std::string &response)
{
    mResponseQueue.push(response);
}

std::string Client::ExtractResponse()
{
    std::string response = mResponseQueue.front();
    mResponseQueue.pop();

    return response;
}

std::string Client::GetClientInfo() const
{
    std::stringstream clientInfo;

    // TODO hostname 수정
    clientInfo << ":" << mNickName << "!" << mUserName << "@" << mHostName;
    return clientInfo.str();
}

void Client::SetChannel(Channel *channel)
{
    mChannel = channel;
}

Channel *Client::GetChannel() const
{
    return mChannel;
}

void Client::SetNickName(const std::string &nickName)
{
    mNickName = nickName;
}

void Client::SetUserName(const std::string &userName)
{
    mUserName = userName;
}

void Client::SetHostName(const std::string &hostName)
{
    mHostName = hostName;
}

void Client::SetServerName(const std::string &serverName)
{
    mServerName = serverName;
}

void Client::SetRealName(const std::string &realName)
{
    mRealName = realName;
}

const std::string &Client::GetNickName() const
{
    return mNickName;
}

const std::string &Client::GetUserName() const
{
    return mUserName;
}

const std::string &Client::GetHostName() const
{
    return mHostName;
}

const std::string &Client::GetServerName() const
{
    return mServerName;
}

const std::string &Client::GetRealName() const
{
    return mRealName;
}

void Client::SetRegistered()
{
    mbRegistered = true;
}

void Client::SetPasswordEntered()
{
    mbPasswordEntered = true;
}

void Client::SetNickNameEntered()
{
    mbNickNameEntered = true;
}

void Client::SetUserInfoEntered()
{
    mbUserInfoEntered = true;
}

bool Client::HasRegistered() const
{
    return mbRegistered;
}

bool Client::HasEnteredPassword() const
{
    return mbPasswordEntered;
}

bool Client::HasEnteredNickName() const
{
    return mbNickNameEntered;
}

bool Client::HasEnteredUserInfo() const
{
    return mbUserInfoEntered;
}
