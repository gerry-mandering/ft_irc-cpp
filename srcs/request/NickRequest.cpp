#include "NickRequest.hpp"

NickRequest::NickRequest(handle_t socket, const std::string &nickname) : Request(socket), mNickName(nickname)
{
    LOG_TRACE("NickRequest constructor called | " << *this);
}

bool NickRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void NickRequest::SetNickName(const std::string &nickName)
{
    mNickName = nickName;
}

const std::string &NickRequest::GetNickName() const
{
    return mNickName;
}

std::ostream &operator<<(std::ostream &os, const NickRequest &nickRequest)
{
    os << "NickRequest = { NickName: " << nickRequest.mNickName << " }";

    return os;
}
