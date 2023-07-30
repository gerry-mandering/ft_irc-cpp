#include "ModeRequest.hpp"

ModeRequest::ModeRequest(handle_t socket, const std::string &channelName, const std::string &plusMinus,
                         const std::string &modeChar, const std::string &modeArgument)
    : Request(socket), mChannelName(channelName), mPlusMinus(plusMinus), mModeChar(modeChar),
      mModeArgument(modeArgument)
{
    LOG_TRACE("ModeRequest constructor: " << *this);
}

bool ModeRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
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

std::ostream &operator<<(std::ostream &os, const ModeRequest &modeRequest)
{
    os << "ModeRequest - ChannelName: " << modeRequest.mChannelName << ", PlusMinus: " << modeRequest.mPlusMinus
       << ", ModeChar: " << modeRequest.mModeArgument;

    return os;
}
