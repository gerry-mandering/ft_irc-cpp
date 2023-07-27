#pragma once

#include "Client.hpp"
#include "EventHandler.hpp"
#include "SingletonTemplate.hpp"
#include <map>

class ClientRepository : public TSingleton<ClientRepository>
{

  public:
    ClientRepository();
    ~ClientRepository();

    Client *FindBySocket(handle_t socket);
    Client *FindByNickname(std::string nickName);

  private:
    std::map<handle_t, Client *> mSocketToClients;
    std::map<std::string, Client *> mNickNameToClients;
};
