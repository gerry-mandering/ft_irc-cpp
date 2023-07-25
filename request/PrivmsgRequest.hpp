#pragma once

#include <vector>
#include "Request.hpp"

class PrivmsgRequest : public Request {

private:
    std::vector<std::string> mTargets;
    std::string mMessage;

};