#include "UserRequest.hpp"

UserRequest::UserRequest(handle_t socket) : Request(socket) {}

void UserRequest::ValidateRequest() const {

}

Command *UserRequest::BuildCommand() const {
    UserCommand *userCommand = new UserCommand(this);

    return userCommand;
}

void UserRequest::SetUserName(const std::string &userName) {
    mUserName = userName;
}

void UserRequest::SetHostName(const std::string &hostName) {
    mHostName = hostName;
}

void UserRequest::SetServerName(const std::string &serverName) {
    mServerName = serverName;
}

void UserRequest::SetRealName(const std::string &realName) {
    mRealName = realName;
}

const std::string &UserRequest::GetUserName() const {
    return mUserName;
}

const std::string &UserRequest::GetHostName() const {
    return mHostName;
}

const std::string &UserRequest::GetServerName() const {
    return mServerName;
}

const std::string &UserRequest::GetRealName() const {
    return mRealName;
}
