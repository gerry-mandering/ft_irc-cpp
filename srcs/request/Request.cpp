#include "Request.hpp"

Request::Request(handle_t socket) : mSocket(socket)
{
    // ClientRepository *clientRepository = ClientRepository::GetInstance();
    // mClient = clientRepository->FindBySocket(socket);

    // if (!mClient)
    // mClient = clientRepository->CreateClient(socket);
    // TODO: 하단 줄 삭제
    (void)mSocket;
}

Client *Request::GetClient() const
{
    return mClient;
}