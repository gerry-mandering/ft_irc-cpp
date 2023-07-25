#pragma once

#include "Reactor.hpp"

enum
{
    OK = 0,
    ERROR_GENERIC,
    ERROR_IO
};

inline Reactor &g_reactor()
{
    return Singleton<Reactor>::instance();
}