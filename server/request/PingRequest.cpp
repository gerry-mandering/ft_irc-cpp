#include "PingRequest.hpp"

PingRequest::PingRequest(handle_t socket) : Request(socket)
{
}

void PingRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void PingRequest::SetToken(const std::string &token)
{
    mToken = token;
}

const std::string &PingRequest::GetToken() const
{
    return mToken;
}