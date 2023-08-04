#pragma once

#include "SharedPtr.hpp"
#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>

class Client;

class Channel
{

  public:
    Channel(const std::string &name);
    ~Channel();

    const std::string &GetName() const;
    std::string GetClientsList();

    void SetClientLimit(int limit);
    int GetClientCount() const;
    int GetClientLimit() const;

    void SetKey(const std::string &key);
    const std::string &GetKey() const;

    void BroadcastMessage(const std::string &message);
    void BroadcastMessageExcludingRequestor(const std::string &message, const std::string &requestorNickName);

    bool CheckClientIsExist(const std::string &nickName);
    bool CheckClientIsOperator(const std::string &nickName);

    void SetClient(SharedPtr< Client > newClient);
    void RemoveClient(const std::string &nickName);

    void SetOperator(SharedPtr< Client > newOperator);
    void RemoveOperator(const std::string &nickName);

    // TODO 메소드 명 변경
    void AddToInvitedClient(SharedPtr< Client > invitedClient);
    void RemoveFromInvitedClient(const std::string &nickName);
    bool CheckClientIsInvited(const std::string &nickName);

    void SetTopic(const std::string &topic);
    const std::string &GetTopic() const;

    bool IsInviteOnlyMode() const;
    bool IsProtectedTopicMode() const;
    bool IsKeyMode() const;
    bool IsClientLimitMode() const;

    void ToggleInviteOnlyMode();
    void ToggleProtectedTopicMode();
    void ToggleKeyMode();
    void ToggleClientLimitMode();

    friend std::stringstream &operator<<(std::stringstream &ss, const Channel &channel);

  private:
    static const int EMPTY_FLAGS = 0;
    static const int INVITE_ONLY_FLAG = 1 << 0;
    static const int PROTECTED_TOPIC_FLAG = 1 << 1;
    static const int KEY_FLAG = 1 << 2;
    static const int CLIENT_LIMIT_FLAG = 1 << 3;

    std::string mName;

    std::vector< SharedPtr< Client > > mClients;
    std::vector< SharedPtr< Client > > mOperators;

    std::vector< SharedPtr< Client > > mInvitedClients;
    std::string mTopic;
    std::string mKey;
    int mClientLimit;

    int mModeFlags;
};
