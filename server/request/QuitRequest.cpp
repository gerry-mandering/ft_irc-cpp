#include "QuitRequest.hpp"

QuitRequest::QuitRequest(handle_t socket) : Request(socket)
{
}

void QuitRequest::Accept(visitor_pattern::Visitor *visitor)
{
    visitor->Visit(this);
}

void QuitRequest::SetReason(const std::string &reason)
{
    mReason = reason;
}

const std::string &QuitRequest::GetReason() const
{
    return mReason;
}