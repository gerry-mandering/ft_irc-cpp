#include "Request.hpp"

Request::Request(handle_t socket) : mSocket(socket)
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    mClient = clientRepository->FindBySocket(socket);

    if (!mClient)
        mClient = clientRepository->CreateClient(socket);
}

SharedPtr< Client > Request::GetClient() const
{
    return mClient;
}
