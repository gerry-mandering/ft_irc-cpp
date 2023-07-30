#include "ClientRepository.hpp"

// TODO: dahkang Register NickName 함수가 필요할듯?, NickToClient map 에 등록하는 함수가 없음 

ClientRepository::ClientRepository() {}

ClientRepository::~ClientRepository() {}

Client *ClientRepository::CreateClient(handle_t socket)
{
    Client *client = new Client(socket);

    mSocketToClients[socket] = client;
    return client;
}

void ClientRepository::RemoveClient(handle_t socket, const std::string &nickName)
{
    std::map<handle_t, Client *>::iterator socketIter = mSocketToClients.find(socket);
    mSocketToClients.erase(socketIter);

    std::map<std::string, Client *>::iterator nickNameIter = mNickNameToClients.find(nickName);
    mNickNameToClients.erase(nickNameIter);
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