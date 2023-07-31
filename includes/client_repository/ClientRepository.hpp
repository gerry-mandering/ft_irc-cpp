#pragma once

#include "Client.hpp"
#include "EventHandler.hpp"
#include "LoggingHandler.hpp"
#include "SingletonTemplate.hpp"
#include <map>

class ClientRepository : public TSingleton<ClientRepository>
{

  public:
    ClientRepository();
    ~ClientRepository();

    Client *CreateClient(handle_t socket);
    void AddClientToNickNameMap(Client *client);

    void RemoveClient(handle_t socket, const std::string &nickName);
    void RemoveClientFromNickNameMap(const std::string &nickName);

    Client *FindBySocket(handle_t socket);
    Client *FindByNickName(std::string nickName);

    int GetNumberOfClients() const;

  private:
    std::map<handle_t, Client *> mSocketToClients;
    std::map<std::string, Client *> mNickNameToClients;
};
