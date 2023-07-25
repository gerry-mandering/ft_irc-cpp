#pragma once

#include "Request.hpp"

class UserRequest : public Request {

private:
    std::string mUsername;
    std::string mHostname;
    std::string mServername;
    std::string mRealName;

};
