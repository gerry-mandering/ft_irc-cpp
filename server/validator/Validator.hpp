#pragma once

//TODO enum 필요 없는 코드 일 수도?
typedef enum eErrorType {
    ACCESS_DENIED,
    ERR_ALREADYREGISTRED = 462,
} eErrorType;

#include <sstream>
#include "Client.hpp"

class CapRequest;
class InviteRequest;
class JoinRequest;
class KickRequest;
class ModeRequest;
class NickRequest;
class PartRequest;
class PassRequest;
class PingRequest;
class PrivmsgRequest;
class QuitRequest;
class TopicRequest;
class UserRequest;

class Validator {

public:
    virtual bool Validate(CapRequest *capRequest) const = 0;
    virtual bool Validate(InviteRequest *inviteRequest) const = 0;
    virtual bool Validate(JoinRequest *joinRequest) const = 0;
    virtual bool Validate(KickRequest *kickRequest) const = 0;
    virtual bool Validate(ModeRequest *modeRequest) const = 0;
    virtual bool Validate(NickRequest *nickRequest) const = 0;
    virtual bool Validate(PartRequest *partRequest) const = 0;
    virtual bool Validate(PassRequest *passRequest) const = 0;
    virtual bool Validate(PingRequest *pingRequest) const = 0;
    virtual bool Validate(PrivmsgRequest *privmsgRequest) const = 0;
    virtual bool Validate(QuitRequest *quitRequest) const = 0;
    virtual bool Validate(TopicRequest *topicRequest) const = 0;
    virtual bool Validate(UserRequest *userRequest) const = 0;
    
protected:
    virtual ~Validator() {}
    void AlreadyRegisteredError(const std::string &nickName) const;
    void AccessDeniedError(const std::string &userName, const std::string &hostName) const;

    void SetClient();

private:
    Client *mClient;

};