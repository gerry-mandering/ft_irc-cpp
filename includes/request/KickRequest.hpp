#pragma once

#include "Request.hpp"
#include <vector>

class KickRequest : public Request
{

  public:
    KickRequest(handle_t socket, const std::string &channelName, const std::vector<std::string> &nickNames,
                const std::string &message);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetChannelName(const std::string &channelName);
    void SetNickName(const std::string &nickName);
    void SetMessage(const std::string &message);

    const std::string &GetChannelName() const;
    std::vector<std::string> &GetNickNames();
    const std::string &GetMessage() const;

    friend std::ostream &operator<<(std::ostream &os, KickRequest &kickRequest);

  private:
    std::string mChannelName;
    std::vector<std::string> mNickNames;
    std::string mMessage;
};
