#ifndef CLIENT_REPOSITORY_H
#define CLIENT_REPOSITORY_H

#include <map>
#include "../singleton_template/SingletonTemplate.hpp"
#include "../../EventHandler.hpp"
#include "../client/Client.hpp"

class ClientRepository : public TSingleton<ClientRepository> {

public:
    ClientRepository();
    ~ClientRepository();

    Client *findBySocket(handle_t socket) const;
    Client *findByNickname(std::string nickname) const;

private:
    std::map<handle_t, Client *> mSocketToClients;
    std::map<std::string, Client *> mNicknameToClients;

};

#endif