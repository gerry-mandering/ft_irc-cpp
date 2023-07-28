#include "Request.hpp"

Request::Request(handle_t socket) : mSocket(socket)
{
    // TODO findBySocket 내부에 없으면 만들어서 반환 로직
    mClient = ClientRepository::GetInstance()->findBySocket(socket);
}

Client *Request::GetClient() const
{
    return mClient;
}