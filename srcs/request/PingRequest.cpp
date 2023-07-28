#include "PingRequest.hpp"

PingRequest::PingRequest(handle_t socket, const std::string &token) : Request(socket), mToken(token) {}

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
