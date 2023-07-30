#include "Request.hpp"

Request::Request(handle_t socket) : mSocket(socket)
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    mClient = clientRepository->FindBySocket(socket);

    if (!mClient)
        mClient = clientRepository->CreateClient(socket);
}

Client *Request::GetClient() const
{
    return mClient;
}

std::ostream &operator<<(std::ostream &os, const Request &request)
{
    os << "Request - Socket: " << request.mSocket;

    return os;
}
