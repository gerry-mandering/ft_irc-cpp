#include "ClientRepository.hpp"

ClientRepository::ClientRepository() {}

ClientRepository::~ClientRepository() {}

//요청이 들어오면 소켓을 무조건 존재 -> 해당 클라이언트를 만들어야함
Client *ClientRepository::FindBySocket(handle_t socket) const {
    std::map<handle_t, Client *>::iterator it = mSocketToClients.find(socket);

    if (it != mSocketToClients.end())
        return *it;

    //TODO shared_ptr로 수정하기
    Client *client = new Client(socket);
    mSocketToClients[socket] = client;
    return client;
}

//없는 닉네임을 조회하는 요청이 들어오면 안 만들어야함
Client *ClientRepository::FindByNickname(std::string nickName) const {
    std::map<std::string, Client *>::iterator it = mNickNameToClients.find(nickName);

    if (it != mNickNameToClients.end())
        return *it;

    return NULL;
}