#include "PassRequest.hpp"

PassRequest::PassRequest(handle_t socket, const std::string &password) : Request(socket), mPassword(password) {}

void PassRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void PassRequest::SetPassword(const std::string &password)
{
    mPassword = password;
}

const std::string &PassRequest::GetPassword() const
{
    return mPassword;
}
