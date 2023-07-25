#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

class Client {

public:
    Client();
    ~Client();
    Client(const Client &other);
    Client &operator=(const Client &other);

private:
    //Builder 패턴 적용
    std::string mIpAddress;
    std::string mNickname;
    std::string mUsername;
    std::string mHostname;
    std::string mServername;
    std::string mRealname;

    bool mbRegisteredFlag;

};

#endif