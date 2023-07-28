#include "ClientRepository.hpp"

ClientRepository::ClientRepository() {}

ClientRepository::~ClientRepository() {}

Client *ClientRepository::CreateClient(handle_t socket)
{
    Client *client = new Client(socket);

    mSocketToClients[socket] = client;
    return client;
}

Client *ClientRepository::FindBySocket(handle_t socket)
{
    if (mSocketToClients.find(socket) != mSocketToClients.end())
        return mSocketToClients[socket];

    // TODO shared_ptr로 수정하기
    return NULL;
}

Client *ClientRepository::FindByNickname(std::string nickName)
{
    if (mNickNameToClients.find(nickName) != mNickNameToClients.end())
        return mNickNameToClients[nickName];

    return NULL;
}