#pragma once

#include "Reactor.hpp"

enum
{
    OK = 0,
    ERROR_GENERIC,
    ERROR_IO
};

// TODO: g_reactor 대신 getInstance로 대체
inline Reactor &g_reactor()
{
    return Singleton< Reactor >::instance();
}