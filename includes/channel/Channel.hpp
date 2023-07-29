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
    void RemoveClient(const std::string &nickName);

    void SetOperator(Client *newOperator);
    void RemoveOperator(const std::string &nickName);

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
    const int INVITE_ONLY_FLAG = 1 << 0;
    const int PROTECTED_TOPIC_FLAG = 1 << 1;
    const int KEY_FLAG = 1 << 2;
    const int CLIENT_LIMIT_FLAG = 1 << 3;

    std::string mName;

    std::vector<Client *> mClients;
    std::vector<Client *> mOperators;

    std::string mTopic;

    int mModeFlags;
};
