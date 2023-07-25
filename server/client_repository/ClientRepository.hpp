#ifndef CLIENT_REPOSITORY_H
#define CLIENT_REPOSITORY_H

#include <map>
#include "../singleton_template/SingletonTemplate.hpp"
#include "../reactor_pattern/ReactorPattern.hpp"
#include "../client/Client.hpp"

class ClientRepository : public T {

public:
    ClientRepository();
    ~ClientRepository();

    Client *findBySocket(reactor::Socket) const;
    Client *findByNickname(std::string nickname) const;

private:
    std::map<reactor::Socket, Client *> mSocketToClients;
    std::map<std::string, Client *> mNicknameToClients;

};

#endif