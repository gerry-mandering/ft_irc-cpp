#pragma once

#include "Client.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

class Channel
{

  public:
    Channel(const std::string &name);

    void BroadcastMessage(const std::string &message);

    bool CheckClientIsExist(const std::string &nickName);
    bool CheckClientIsOperator(const std::string &nickName);

    void SetClient(Client *newClient);
    void RemoveClient(Client *clientToRemove);

    const std::string &GetTopic() const;
    void SetTopic(const std::string &topic);

    bool IsInviteOnlyMode() const;
    bool IsProtectedTopicMode() const;
    bool IsKeyMode() const;
    bool IsClientLimitMode() const;

    void ToggleInviteOnlyMode();
    void ToggleProtectedTopicMode();
    void ToggleKeyMode();
    void ToggleClientLimitMode();

  private:
    std::string mName;

    std::vector<Client *> mClients;
    std::vector<Client *> mOperators;

    std::string mTopic;

    bool mbInviteOnlyMode;
    bool mbProtectedTopicMode;
    bool mbKeyMode;
    bool mbClientLimitMode;
};
