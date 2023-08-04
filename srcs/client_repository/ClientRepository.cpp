#include "ClientRepository.hpp"

ClientRepository::ClientRepository() {}

ClientRepository::~ClientRepository() {}

SharedPtr< Client > ClientRepository::CreateClient(handle_t socket)
{
    SharedPtr< Client > client(new Client(socket));

    mSocketToClients[socket] = client;

    LOG_TRACE("ClientRepository CreateClient()");

    return client;
}

void ClientRepository::AddClientToNickNameMap(SharedPtr< Client > client)
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

SharedPtr< Client > ClientRepository::FindBySocket(handle_t socket)
{
    if (mSocketToClients.find(socket) != mSocketToClients.end())
    {
        LOG_TRACE("ClientRepository FindBySocket() - Found");
        return mSocketToClients[socket];
    }

    LOG_TRACE("ClientRepository FindBySocket() - NotFound");

    return SharedPtr< Client >();
}

SharedPtr< Client > ClientRepository::FindByNickName(std::string nickName)
{
    if (mNickNameToClients.find(nickName) != mNickNameToClients.end())
    {
        LOG_TRACE("ClientRepository FindByNickName() - Found");
        return mNickNameToClients[nickName];
    }

    LOG_TRACE("ClientRepository FindByNickName() - NotFound");

    return SharedPtr< Client >();
}

int ClientRepository::GetNumberOfClients() const
{
    return mSocketToClients.size();
}