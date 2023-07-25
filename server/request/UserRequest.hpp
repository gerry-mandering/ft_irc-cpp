#pragma once

#include "Request.hpp"
#include "../command/UserCommand.hpp"

class UserRequest : public Request {

public:
    UserRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetUserName(const std::string &userName);
    void SetHostName(const std::string &hostName);
    void SetServerName(const std::string &serverName);
    void SetRealName(const std::string &realName);

    const std::string &GetUserName() const;
    const std::string &GetHostName() const;
    const std::string &GetServerName() const;
    const std::string &GetRealName() const;

private:
    std::string mUserName;
    std::string mHostName;
    std::string mServerName;
    std::string mRealName;

};
