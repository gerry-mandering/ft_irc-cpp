#include "ClientRepository.hpp"

ClientRepository::ClientRepository() {}

ClientRepository::~ClientRepository() {}

Client *ClientRepository::CreateClient(handle_t socket)
{
    SharedPtr< Client > client(new Client(socket));

    mSocketToClients[socket] = client;

    LOG_TRACE("ClientRepository CreateClient()");

    return client.GetPtr();
}

void ClientRepository::AddClientToNickNameMap(Client *client)
{
    mNickNameToClients[client->GetNickName()] = SharedPtr< Client >(client);

    LOG_TRACE("ClientRepository AddClientToNickNameMap()");
}

void ClientRepository::RemoveClient(handle_t socket, const std::string &nickName)
{
    std::map< handle_t, SharedPtr< Client > >::iterator socketIter = mSocketToClients.find(socket);
    mSocketToClients.erase(socketIter);

    std::map< std::string, SharedPtr< Client > >::iterator nickNameIter = mNickNameToClients.find(nickName);

    if (nickNameIter != mNickNameToClients.end())
        mNickNameToClients.erase(nickNameIter);

    LOG_TRACE("ClientRepository RemoveClient()");
}

void ClientRepository::RemoveClientFromNickNameMap(const std::string &nickName)
{
    std::map< std::string, SharedPtr< Client > >::iterator iter = mNickNameToClients.find(nickName);

    mNickNameToClients.erase(iter);

    LOG_TRACE("ClientRepository RemoveClientFromNickNameMap()");
}

Client *ClientRepository::FindBySocket(handle_t socket)
{
    if (mSocketToClients.find(socket) != mSocketToClients.end())
    {
        LOG_TRACE("ClientRepository FindBySocket() - Found");
        return mSocketToClients[socket].GetPtr();
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
        return mNickNameToClients[nickName].GetPtr();
    }

    LOG_TRACE("ClientRepository FindByNickName() - NotFound");

    return NULL;
}

int ClientRepository::GetNumberOfClients() const
{
    return mSocketToClients.size();
}