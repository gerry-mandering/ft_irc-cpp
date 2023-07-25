#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <vector>
#include "../client/Client.hpp"

class Channel {

public:
    Channel();
    ~Channel();

    std::vector<Client *> &GetClients() const;

private:
    std::string mName;

    std::vector<Client *> mClients;

};

#endif