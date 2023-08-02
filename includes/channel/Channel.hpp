#pragma once

#include "Client.hpp"
#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>

class Channel
{

    // TODO: dahkang 소멸자 정의할것
  public:
    Channel(const std::string &name);

    const std::string &GetName() const;
    std::vector<Client *> GetClients();

    void SetClientLimit(int limit);
    int GetClientCount() const;
    int GetClientLimit() const;

    void SetKey(const std::string &key);
    const std::string &GetKey() const;

    void BroadcastMessage(const std::string &message);
    void BroadcastMessageExcludingRequestor(const std::string &message, const std::string &requestorNickName);

    bool CheckClientIsExist(const std::string &nickName);
    bool CheckClientIsOperator(const std::string &nickName);

    void SetClient(Client *newClient);
    void RemoveClient(const std::string &nickName);

    void SetOperator(Client *newOperator);
    void RemoveOperator(const std::string &nickName);

    void AddToInvitedClient(Client *invitedClient);
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

    friend std::ostream &operator<<(std::ostream &os, const Channel &channel);

  private:
    static const int EMPTY_FLAGS = 0;
    static const int INVITE_ONLY_FLAG = 1 << 0;
    static const int PROTECTED_TOPIC_FLAG = 1 << 1;
    static const int KEY_FLAG = 1 << 2;
    static const int CLIENT_LIMIT_FLAG = 1 << 3;

    std::string mName;

    // TODO: dahkang 현재 mClients와 mOperators 두곳에 오퍼레이터가 중복해서 포함되는 것같은데, 어차피 채널 하나밖에
    // 접속 못하니 차라리 client에 operator 플래그를 두고 합치는 게 어떨까?
    // 지금 구현은 operator가 양쪽 벡터 모두에 속해있는 것 같은데 실수할 확률 좀 있어보임
    std::vector<Client *> mClients;
    std::vector<Client *> mOperators;

    // 닉네임이 변경되어도 InviteOnly 모드에서 채널 진입이 가능하므로 Client *사용
    std::vector<Client *> mInvitedClients;
    std::string mTopic;
    std::string mKey;
    int mClientLimit;

    int mModeFlags;
};
