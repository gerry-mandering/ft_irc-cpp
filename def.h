#pragma once

#include "Reactor.hpp"

enum
{
    OK = 0,
    ERROR_GENERIC,
    ERROR_IO
};

#define g_reactor Reactor::instance()