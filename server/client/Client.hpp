#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include "../../EventHandler.hpp"

class Client {

public:
    Client(handle_t socket);

private:
    //Builder 패턴 적용
    std::string mSocket;
    std::string mNickname;
    std::string mUsername;
    std::string mHostname;
    std::string mServername;
    std::string mRealname;

    bool mbRegisteredFlag;

};

#endif