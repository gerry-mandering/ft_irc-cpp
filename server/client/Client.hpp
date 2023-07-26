#pragma once

#include <iostream>
#include "EventHandler.hpp"

class Client {

public:
    Client(handle_t socket);

    void SetNickName(const std::string &nickName);
    void SetUserName(const std::string &userName);
    void SetHostName(const std::string &hostName);
    void SetServerName(const std::string &serverName);
    void SetRealName(const std::string &realName);

    const std::string &GetNickName() const;
    const std::string &GetUserName() const;
    const std::string &GetHostName() const;
    const std::string &GetServerName() const;
    const std::string &GetRealName() const;

    void SetClientRegistered();
    void SetPasswordEntered();
    void SetNickNameEntered();
    void SetUserInfoEntered();

private:
    //Builder 패턴 적용
    std::string mSocket;
    std::string mNickName;
    std::string mUserName;
    std::string mHostName;
    std::string mServerName;
    std::string mRealName;

    bool mbRegisteredClient;
    bool mbEnteredPassword;
    bool mbEnteredNickName;
    bool mbEnteredUserInfo;

};
