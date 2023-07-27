#pragma once

#include "EventHandler.hpp"
#include <iostream>
#include <queue>

class Client
{

  public:
    Client(handle_t socket);

    void InsertResponse(const std::string &response);
    const std::string ExtractResponse();

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

    bool Registered() const;
    bool EnteredPassword() const;
    bool EnteredNickName() const;
    bool EnteredUserInfo() const;

  private:
    // Builder 패턴 적용
    handle_t mSocket;
    std::queue<std::string> mResponseQueue;

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
