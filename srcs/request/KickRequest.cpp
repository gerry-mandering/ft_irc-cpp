#include "KickRequest.hpp"

KickRequest::KickRequest(handle_t socket, const std::vector<std::string> &nickNames, const std::string &message)
    : Request(socket), mNickNames(nickNames), mMessage(message)
{
    //    Logger::GetInstance()->Trace("KickRequest constructor", *this);
}

bool KickRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void KickRequest::SetNickName(const std::string &nickName)
{
    mNickNames.push_back(nickName);
}

void KickRequest::SetMessage(const std::string &message)
{
    mMessage = message;
}

std::vector<std::string> &KickRequest::GetNickNames()
{
    return mNickNames;
}

const std::string &KickRequest::GetMessage() const
{
    return mMessage;
}

// TODO const 아니여도 ㄱㅊ?
std::ostream &operator<<(std::ostream &os, KickRequest &kickRequest)
{
    os << "KickRequest - NickNames: ";

    std::vector<std::string>::iterator iter;
    for (iter = kickRequest.mNickNames.begin(); iter != kickRequest.mNickNames.end(); iter++)
        os << *iter << ", ";

    os << "Message: " << kickRequest.mMessage;

    return os;
}
