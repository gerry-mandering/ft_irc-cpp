#pragma once

#include <iostream>
#include <vector>
#include "Client.hpp"

class Channel {

public:
    Channel(const std::string &name);

    void SetClient(Client *newClient);

    std::vector<Client *> GetClients() const;
    std::vector<Client *> GetOperators() const;

private:
    std::string mName;

    std::vector<Client *> mClients;
    std::vector<Client *> mOperators;

    bool mbInviteOnlyMode;
    bool mbProtectedTopicMode;
    bool mbKeyMode;
    bool mbClientLimitMode;

};
