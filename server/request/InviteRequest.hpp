#pragma once

#include "Request.hpp"
#include "InviteCommand.hpp"

class InviteRequest : public Request {

public:
    InviteRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetNickName(const std::string &nickName);
    void SetChannelName(const std::string &channelName);

    const std::string &GetNickName() const;
    const std::string &GetChannelName() const;

private:
    std::string mNickName;
    std::string mChannelName;

};