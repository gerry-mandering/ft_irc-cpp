#pragma once

#include "Request.hpp"

class QuitRequest : public Request {

private:
    std::string mReason;

};