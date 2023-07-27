#pragma once

#include "Request.hpp"
#include <vector>

class JoinRequest : public Request
{

  public:
    JoinRequest(handle_t socket);

    void Accept(visitor_pattern::Visitor *visitor);

    void SetChannelName(const std::string &channelName);
    void SetKey(const std::string &key);

    const std::vector<std::string> &GetChannelNames() const;
    const std::vector<std::string> &GetKeys() const;

  private:
    std::vector<std::string> mChannelNames;
    std::vector<std::string> mKeys;
};