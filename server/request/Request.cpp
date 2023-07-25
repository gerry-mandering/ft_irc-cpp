#include "Request.hpp"

Request::Request(reactor::Socket socket) {
    //findBySocket 내부에 없으면 만들어서 반환 로직
   mClient = ClientRepository::GetInstance()->findBySocket(socket);
}

Request::~Request() {}
