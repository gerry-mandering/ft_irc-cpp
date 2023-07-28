#include "Validator.hpp"

// TODO EnvManager 고치기
#define PASSWORD "1234"

// TODO registered 관련된거 아니면 다 reistered 했는지 체크하기
bool Validator::Visit(CapRequest *capRequest) const {}

bool Validator::Visit(InviteRequest *inviteRequest) const {}

bool Validator::Visit(JoinRequest *joinRequest) const {}

bool Validator::Visit(KickRequest *kickRequest) const {}

bool Validator::Visit(ModeRequest *modeRequest) const {}

bool Validator::Visit(NickRequest *nickRequest) const
{
    ClientRepository *clientRepository = ClientRepository::GetInstance();
    Client *client = nickRequest->GetClient();

    // 이미 해당 닉네임을 사용하는 클라이언트가 존재하는 경우
    if (clientRepository->FindByNickname(nickRequest->GetNickName()) != NULL)
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildNickNameInUseMsg(nickRequest->GetNickName(), client->GetNickName());
        else
            errorMessage = BuildNickNameInUseMsg(nickRequest->GetNickName());

        client->InsertResponse(errorMessage);
        return false;
    }

    // Password를 입력하지 않았는데 register를 시도하는 경우
    if (client->HasEnteredUserInfo() && !client->HasEnteredPassword())
    {
        std::string errorMessage;

        if (client->HasEnteredUserInfo())
            errorMessage = BuildAccessDeniedMsg(client->GetUserName(), client->GetHostName());
        else
            errorMessage = BuildAccessDeniedMsg();

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

bool Validator::Visit(PartRequest *partRequest) const {}

// Password 틀릴 경우 그냥 바로 Closing Link - 원래는 보관했다가 register 할 때
// 검증
bool Validator::Visit(PassRequest *passRequest) const
{
    Client *client = passRequest->GetClient();

    if (client->HasRegistered())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildAlreadyRegisteredMsg(client->GetNickName());
        else
            errorMessage = BuildAlreadyRegisteredMsg();

        client->InsertResponse(errorMessage);
        return true;
    }

    if (client->HasEnteredPassword())
        return true;

    if (passRequest->GetPassword() != PASSWORD)
    {
        std::string errorMessage;

        if (client->HasEnteredUserInfo())
            errorMessage = BuildAccessDeniedMsg(client->GetUserName(), client->GetHostName());
        else
            errorMessage = BuildAccessDeniedMsg(); // TODO 사용자 hostname 은 가져와서 넣기

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

bool Validator::Visit(PingRequest *pingRequest) const
{
    // 파라미터가 부족한 경우는 Parser에서 처리

    // Error 종류 중에 ERR_NOORIGIN (409)는 처리할 필요 없는 듯?

    return true;
}

bool Validator::Visit(PrivmsgRequest *privmsgRequest) const {}

bool Validator::Visit(QuitRequest *quitRequest) const {}

bool Validator::Visit(TopicRequest *topicRequest) const {}

bool Validator::Visit(UserRequest *userRequest) const
{
    Client *client = userRequest->GetClient();

    if (client->HasEnteredUserInfo())
    {
        std::string errorMessage;

        if (client->HasEnteredNickName())
            errorMessage = BuildAlreadyRegisteredMsg(client->GetNickName());
        else
            errorMessage = BuildAlreadyRegisteredMsg();

        client->InsertResponse(errorMessage);
        return false;
    }

    if (client->HasEnteredNickName() && !client->HasEnteredPassword())
    {
        std::string errorMessage;

        // TODO hostname Ip로 알아오기
        errorMessage = BuildAccessDeniedMsg(userRequest->GetUserName(), userRequest->GetHostName());

        client->InsertResponse(errorMessage);
        return false;
    }

    return true;
}

std::string Validator::BuildAlreadyRegisteredMsg(const std::string &nickName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream ErrorMessage;

    // TODO EnvManager에서 서버 이름 가져오기
    ErrorMessage << ":" << envManager->GetServerName() << " 462 " << nickName << ":You may not reregister";

    return ErrorMessage.str();
}

// TODO 원래는 GetHostName()이 아니라 아이피 주소로 출력됨 방법 찾기
// TODO AccessDenied 는 연결을 끊어야 함!!!!!
std::string Validator::BuildAccessDeniedMsg(const std::string &userName, const std::string &hostName)
{
    std::stringstream ErrorMessage;

    ErrorMessage << "Error :Closing link: (" << userName << "@" << hostName << ") [Access denied by configuration]";

    return ErrorMessage.str();
}

std::string Validator::BuildNickNameInUseMsg(const std::string &newNickName, const std::string &clientNickName)
{
    EnvManager *envManager = EnvManager::GetInstance();
    std::stringstream ErrorMessage;

    ErrorMessage << ":" << envManager->GetServerName() << " 433 " << clientNickName << " " << newNickName
                 << " :Nickname is already in use.";

    return ErrorMessage.str();
}