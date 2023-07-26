#include "Channel.hpp"

Channel::Channel(const std::string &name) : mName(name),
                                            mbInviteOnlyMode(false),
                                            mbProtectedTopicMode(false),
                                            mbKeyMode(false),
                                            mbClientLimitMode(false) {}

void Channel::SetClient(Client *newClient) {
    mClients.push_back(newClient);
}

std::vector<Client *> Channel::GetClients() const {
    return mClients;
}

std::vector<Client *> Channel::GetOperators() const {
    return mOperators;
}