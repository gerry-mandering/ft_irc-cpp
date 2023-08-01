#pragma once

#include "Request.hpp"

class ModeRequest : public Request
{

  public:
    ModeRequest(handle_t socket, const std::string &channelName, const std::string &sign, const std::string &modeChar,
                const std::string &modeArgument);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetChannelName(const std::string &channelName);
    void SetSign(const std::string &sign);
    void SetModeChar(const std::string &modeChar);
    void SetModeArgument(const std::string &modeArgument);

    const std::string &GetChannelName() const;
    const std::string &GetSign() const;
    const std::string &GetModeChar() const;
    const std::string &GetModeArgument() const;

    friend std::ostream &operator<<(std::ostream &os, const ModeRequest &modeRequest);

  private:
    std::string mChannelName;
    std::string mSign;
    std::string mModeChar;
    std::string mModeArgument;
};
