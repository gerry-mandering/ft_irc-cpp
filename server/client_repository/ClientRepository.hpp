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

    Client *FindBySocket(handle_t socket) const;
    Client *FindByNickname(std::string nickName) const;

  private:
    std::map<handle_t, Client *> mSocketToClients;
    std::map<std::string, Client *> mNickNameToClients;
};
