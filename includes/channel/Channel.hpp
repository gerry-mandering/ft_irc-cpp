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

    std::string mTopic;

    int mModeFlags;
};
