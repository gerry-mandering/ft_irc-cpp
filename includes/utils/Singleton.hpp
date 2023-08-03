#pragma once

#include "Reactor.hpp"

template < class T >
class Singleton
{
  public:
    static inline T &instance();
    void release();

  protected:
    Singleton(void) {}
    ~Singleton(void) {}
};

template < class T >
inline T &Singleton< T >::instance()
{
    static T instance;

    return instance;
}
