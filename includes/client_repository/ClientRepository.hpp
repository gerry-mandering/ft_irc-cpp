#pragma once

#include "Client.hpp"
#include "EventHandler.hpp"
#include "LoggingHandler.hpp"
#include "SharedPtr.hpp"
#include "SingletonTemplate.hpp"
#include <map>

class ClientRepository : public TSingleton< ClientRepository >
{

  public:
    ClientRepository();
    ~ClientRepository();

    SharedPtr< Client > CreateClient(handle_t socket);
    void AddClientToNickNameMap(SharedPtr< Client > client);

    void RemoveClient(handle_t socket, const std::string &nickName);
    void RemoveClientFromNickNameMap(const std::string &nickName);

    SharedPtr< Client > FindBySocket(handle_t socket);
    SharedPtr< Client > FindByNickName(std::string nickName);

    int GetNumberOfClients() const;

  private:
    std::map< handle_t, SharedPtr< Client > > mSocketToClients;
    std::map< std::string, SharedPtr< Client > > mNickNameToClients;
};
