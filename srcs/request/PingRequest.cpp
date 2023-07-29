#include "PingRequest.hpp"

PingRequest::PingRequest(handle_t socket, const std::string &token) : Request(socket), mToken(token)
{
    //    Logger::GetInstance()->Trace("PingRequest constructor", *this);
}

bool PingRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void PingRequest::SetToken(const std::string &token)
{
    mToken = token;
}

const std::string &PingRequest::GetToken() const
{
    return mToken;
}

std::ostream &operator<<(std::ostream &os, const PingRequest &pingRequest)
{
    os << "PingRequest - Token: " << pingRequest.mToken;

    return os;
}
