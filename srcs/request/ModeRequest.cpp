#include "ModeRequest.hpp"

ModeRequest::ModeRequest(handle_t socket, const std::string &channelName, const std::string &sign,
                         const std::string &modeChar, const std::string &modeArgument)
    : Request(socket), mChannelName(channelName), mSign(sign), mModeChar(modeChar), mModeArgument(modeArgument)
{
    LOG_TRACE("ModeRequest constructor called | " << *this);
}

bool ModeRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void ModeRequest::SetChannelName(const std::string &channelName)
{
    mChannelName = channelName;
}

void ModeRequest::SetSign(const std::string &sign)
{
    mSign = sign;
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

const std::string &ModeRequest::GetSign() const
{
    return mSign;
}

const std::string &ModeRequest::GetModeChar() const
{
    return mModeChar;
}

const std::string &ModeRequest::GetModeArgument() const
{
    return mModeArgument;
}

std::stringstream &operator<<(std::stringstream &ss, const ModeRequest &modeRequest)
{
    ss << "ModeRequest = { ChannelName: " << modeRequest.mChannelName << ", PlusMinus: " << modeRequest.mSign
       << ", ModeChar: " << modeRequest.mModeArgument << " }";

    return ss;
}
