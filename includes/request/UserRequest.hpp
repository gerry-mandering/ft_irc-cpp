#pragma once

#include "Request.hpp"

class UserRequest : public Request
{

  public:
    UserRequest(handle_t socket, const std::string &userName, const std::string &hostName,
                const std::string &serverName, const std::string &realName);

    bool Accept(visitor_pattern::Visitor *visitor);

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
