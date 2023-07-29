#pragma once

#include "Request.hpp"

class ModeRequest : public Request
{

  public:
    ModeRequest(handle_t socket, const std::string &channelName, const std::string &plusMinus,
                const std::string &modeChar, const std::string &modeArgument);

    bool Accept(visitor_pattern::Visitor *visitor);

    void SetChannelName(const std::string &channelName);
    void SetPlusMinus(const std::string &plusMinus);
    void SetModeChar(const std::string &modeChar);
    void SetModeArgument(const std::string &modeArgument);

    const std::string &GetChannelName() const;
    const std::string &GetPlusMinus() const;
    const std::string &GetModeChar() const;
    const std::string &GetModeArgument() const;

    friend std::ostream &operator<<(std::ostream &os, const ModeRequest &modeRequest);

  private:
    std::string mChannelName;
    std::string mPlusMinus;
    std::string mModeChar;
    std::string mModeArgument;
};
