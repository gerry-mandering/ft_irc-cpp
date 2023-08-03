#pragma once

#include "Request.hpp"

class InviteRequest : public Request
{

  public:
    InviteRequest(handle_t socket, const std::string &nickName, const std::string &channelName);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetNickName(const std::string &nickName);
    void SetChannelName(const std::string &channelName);

    const std::string &GetNickName() const;
    const std::string &GetChannelName() const;

    friend std::stringstream &operator<<(std::stringstream &ss, const InviteRequest &inviteRequest);

  private:
    std::string mNickName;
    std::string mChannelName;
};