#pragma once

#include "Request.hpp"
#include <vector>

class JoinRequest : public Request
{

  public:
    JoinRequest(handle_t socket, const std::string &mChannelName, const std::string &mKey);
    bool Accept(visitor_pattern::Visitor *visitor);

    void setChannelName(const std::string &channelName);
    void setKey(const std::string &key);

    const std::string &getChannelName() const;
    const std::string &getKey() const;

    friend std::ostream &operator<<(std::ostream &os, const JoinRequest &joinRequest);

  private:
    std::string mChannelName;
    std::string mKey;
};