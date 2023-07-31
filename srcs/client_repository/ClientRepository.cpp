#include "ClientRepository.hpp"

// TODO: dahkang Register NickName 함수가 필요할듯?, NickToClient map 에 등록하는 함수가 없음

ClientRepository::ClientRepository() {}

ClientRepository::~ClientRepository() {}

Client *ClientRepository::CreateClient(handle_t socket)
{
    Client *client = new Client(socket);

    mSocketToClients[socket] = client;

    LOG_TRACE("ClientRepository CreateClient()");

    return client;
}

void ClientRepository::AddClientToNickNameMap(Client *client)
{
    mNickNameToClients[client->GetNickName()] = client;

    LOG_TRACE("ClientRepository AddClientToNickNameMap()");
}

void ClientRepository::RemoveClient(handle_t socket, const std::string &nickName)
{
    std::map<handle_t, Client *>::iterator socketIter = mSocketToClients.find(socket);
    mSocketToClients.erase(socketIter);

    std::map<std::string, Client *>::iterator nickNameIter = mNickNameToClients.find(nickName);

    if (nickNameIter != mNickNameToClients.end())
        mNickNameToClients.erase(nickNameIter);

    LOG_TRACE("ClientRepository RemoveClient()");
}

void ClientRepository::RemoveClientFromNickNameMap(const std::string &nickName)
{
    std::map<std::string, Client *>::iterator iter = mNickNameToClients.find(nickName);

    mNickNameToClients.erase(iter);

    LOG_TRACE("ClientRepository RemoveClientFromNickNameMap()");
}

Client *ClientRepository::FindBySocket(handle_t socket)
{

    if (mSocketToClients.find(socket) != mSocketToClients.end())
    {
        LOG_TRACE("ClientRepository FindBySocket() - Found");
        return mSocketToClients[socket];
    }

    // TODO shared_ptr로 수정하기
    LOG_TRACE("ClientRepository FindBySocket() - NotFound");

    return NULL;
}

Client *ClientRepository::FindByNickName(std::string nickName)
{
    if (mNickNameToClients.find(nickName) != mNickNameToClients.end())
    {
        LOG_TRACE("ClientRepository FindByNickName() - Found");
        return mNickNameToClients[nickName];
    }

    LOG_TRACE("ClientRepository FindByNickName() - NotFound");

    return NULL;
}

int ClientRepository::GetNumberOfClients() const
{
    return mSocketToClients.size();
}