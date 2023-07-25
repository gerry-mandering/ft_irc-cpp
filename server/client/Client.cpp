#include "Client.hpp"

Client::Client(handle_t socket) : mSocket(socket),
                                  mbRegisteredClient(false),
                                  mbEnteredPassword(false),
                                  mbEnteredNickName(false),
                                  mbEnteredUserInfo(false) {}

void Client::SetNickName(const std::string &nickName) {
    mNickName = nickName;
}

void Client::SetUserName(const std::string &userName) {
    mUserName = userName;
}

void Client::SetHostName(const std::string &hostName) {
    mHostName = hostName;
}

void Client::SetServerName(const std::string &serverName) {
    mServerName = serverName;
}

void Client::SetRealName(const std::string &realName) {
    mRealName = realName;
}

const std::string &Client::GetNickName() const {
    return mNickName;
}

const std::string &Client::GetUserName() const {
    return mUserName;
}

const std::string &Client::GetHostName() const {
    return mHostName;
}

const std::string &Client::GetServerName() const {
    return mServerName;
}

const std::string &Client::GetRealName() const {
    return mRealName;
}

void Client::SetClientRegistered() {
    mbRegisteredClient = true;
}

void Client::SetPasswordEntered() {
    mbEnteredPassword = true;
}

void Client::SetNickNameEntered() {
    mbEnteredNickName = true;
}

void Client::SetUserInfoEntered() {
    mbEnteredUserInfo = true;
}