#pragma once

#include <stdexcept>

// TODO: dahkang _NOEXCEPT 키워드는 cpp11 cpp98은 nothrow
class AddrInfoException : public std::exception
{

  public:
    const char *what() const _NOEXCEPT { return "Failed to initialize address info."; }
};

class SocketCreationException : public std::exception
{

  public:
    const char *what() const _NOEXCEPT { return "Failed to create socket."; }
};

class BindException : public std::exception
{

  public:
    const char *what() const _NOEXCEPT { return "Failed to bind socket."; }
};

class ListenException : public std::exception
{

  public:
    const char *what() const _NOEXCEPT { return "Failed to put socket in listening state."; }
};
