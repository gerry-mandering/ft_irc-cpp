#include "ModeRequest.hpp"

ModeRequest::ModeRequest(handle_t socket) : Request(socket)
{
}

void ModeRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void ModeRequest::SetChannelName(const std::string &channelName)
{
    mChannelName = channelName;
}

void ModeRequest::SetPlusMinus(const std::string &plusMinus)
{
    mPlusMinus = plusMinus;
}

void ModeRequest::SetModeChar(const std::string &modeChar)
{
    mModeChar = modeChar;
}

void ModeRequest::SetModeArgument(const std::string &modeArgument)
{
    mModeArgument = modeArgument;
}

const std::string &ModeRequest::GetChannelName() const
{
    return mChannelName;
}

const std::string &ModeRequest::GetPlusMinus() const
{
    return mPlusMinus;
}

const std::string &ModeRequest::GetModeChar() const
{
    return mModeChar;
}

const std::string &ModeRequest::GetModeArgument() const
{
    return mModeArgument;
}
