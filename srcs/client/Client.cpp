#include "Client.hpp"

Client::Client(handle_t socket)
    : mSocket(socket), mChannel(NULL), mNickName(std::string()), mUserName(std::string()), mHostName(std::string()),
      mServerName(std::string()), mRealName(std::string()), mRegistrationFlags(0)
{
    LOG_TRACE("Client constructor called | " << *this);
}

void Client::addResponseToBuf(const std::string &response)
{
    mResponseQueue.push(response);
}

std::string Client::ExtractResponse()
{
    std::string response = mResponseQueue.front();
    mResponseQueue.pop();

    return response;
}

bool Client::HasResponse() const
{
    return !mResponseQueue.empty();
}

std::string Client::GetClientInfo() const
{
    std::stringstream clientInfo;

    // TODO hostname 수정
    clientInfo << ":" << mNickName << "!" << mUserName << "@" << mHostName;
    return clientInfo.str();
}

handle_t Client::GetSocket() const
{
    return mSocket;
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
    mRegistrationFlags |= REGISTERED_FLAG;
}

void Client::SetPasswordEntered()
{
    mRegistrationFlags |= PASSWORD_ENTERED_FLAG;
}

void Client::SetNickNameEntered()
{
    mRegistrationFlags |= NICKNAME_ENTERED_FLAG;
}

void Client::SetUserInfoEntered()
{
    mRegistrationFlags |= USER_INFO_ENTERED_FLAG;
}

bool Client::HasRegistered() const
{
    return (mRegistrationFlags & REGISTERED_FLAG) != 0;
}

bool Client::HasEnteredPassword() const
{
    return (mRegistrationFlags & PASSWORD_ENTERED_FLAG) != 0;
}

bool Client::HasEnteredNickName() const
{
    return (mRegistrationFlags & NICKNAME_ENTERED_FLAG) != 0;
}

bool Client::HasEnteredUserInfo() const
{
    return (mRegistrationFlags & USER_INFO_ENTERED_FLAG) != 0;
}

std::ostream &operator<<(std::ostream &os, const Client &client)
{
    os << "Client = { Socket: " << client.mSocket << ", ResponseQueue.size: " << client.mResponseQueue.size()
       << ", Channel: " << client.mChannel << ", NickName: " << client.mNickName << ", UserName: " << client.mUserName
       << ", HostName: " << client.mHostName << ", ServerName: " << client.mServerName
       << ", RealName: " << client.mRealName << ", RegistrationFlags: " << std::bitset<4>(client.mRegistrationFlags)
       << " }";

    return os;
}
