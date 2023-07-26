#pragma once

#include "Request.hpp"
#include "ModeCommand.hpp"

class ModeRequest : public Request {

public:
    ModeRequest(handle_t socket);

    void ValidateRequest() const;
    Command *BuildCommand() const;

    void SetChannelName(const std::string &channelName);
    void SetPlusMinus(const std::string &plusMinus);
    void SetModeChar(const std::string &modeChar);
    void SetModeArgument(const std::string &modeArgument);

    const std::string &GetChannelName() const;
    const std::string &GetPlusMinus() const;
    const std::string &GetModeChar() const;
    const std::string &GetModeArgument() const;

private:
    std::string mChannelName;
    std::string mPlusMinus;
    std::string mModeChar;
    std::string mModeArgument;

};
