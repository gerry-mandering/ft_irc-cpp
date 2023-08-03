#include "PassRequest.hpp"

PassRequest::PassRequest(handle_t socket, const std::string &password) : Request(socket), mPassword(password)
{
    LOG_TRACE("PassRequest constructor called | " << *this);
}

bool PassRequest::Accept(visitor_pattern::Visitor *visitor)
{
    return visitor->Visit(this);
}

void PassRequest::SetPassword(const std::string &password)
{
    mPassword = password;
}

const std::string &PassRequest::GetPassword() const
{
    return mPassword;
}

std::stringstream &operator<<(std::stringstream &ss, const PassRequest &passRequest)
{
    ss << "PassRequest = { Password: " << passRequest.mPassword << " }";

    return ss;
}
