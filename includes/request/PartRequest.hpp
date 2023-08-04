#pragma once

#include "Request.hpp"

class PartRequest : public Request
{

  public:
    PartRequest(handle_t socket, const std::string &channelName, const std::string &reason);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetChannelName(const std::string &channelName);
    void SetReason(const std::string &reason);

    const std::string &GetChannelName() const;
    const std::string &GetReason() const;

    friend std::stringstream &operator<<(std::stringstream &ss, const PartRequest &partRequest);

  private:
    std::string mChannelName;
    std::string mReason;
};
