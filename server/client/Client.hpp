#pragma once

#include "EventHandler.hpp"
#include <iostream>
#include <queue>
#include <sstream>

class Channel;

class Client
{

  public:
    Client(handle_t socket);

    void InsertResponse(const std::string &response);
    std::string ExtractResponse();

    std::string GetClientInfo() const;

    void SetChannel(Channel *channel);
    Channel *GetChannel() const;

    void SetNickName(const std::string &nickName);
    void SetUserName(const std::string &userName);
    void SetHostName(const std::string &hostName);
    void SetServerName(const std::string &serverName);
    void SetRealName(const std::string &realName);

    void SetRegistered();
    void SetPasswordEntered();
    void SetNickNameEntered();
    void SetUserInfoEntered();

    const std::string &GetNickName() const;
    const std::string &GetUserName() const;
    const std::string &GetHostName() const;
    const std::string &GetServerName() const;
    const std::string &GetRealName() const;

    bool HasRegistered() const;
    bool HasEnteredPassword() const;
    bool HasEnteredNickName() const;
    bool HasEnteredUserInfo() const;

  private:
    // Builder 패턴 적용
    handle_t mSocket;
    std::queue<std::string> mResponseQueue;

    Channel *mChannel;

    std::string mNickName;
    std::string mUserName;
    std::string mHostName;
    std::string mServerName;
    std::string mRealName;

    bool mbRegistered;
    bool mbPasswordEntered;
    bool mbNickNameEntered;
    bool mbUserInfoEntered;
};
