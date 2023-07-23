#include "StreamHandler.hpp"
#include "def.h"
#include <cstring>
#include <iostream>
#include <unistd.h>

StreamHandler::StreamHandler(handle_t handle) : EventHandler(handle) {}

StreamHandler::~StreamHandler() {}

int StreamHandler::handleRead(void)
{
    char buf[1024];

    std::memset(buf, 0, sizeof(buf));
    if (read(m_handle, buf, sizeof(buf)) < 0)
    {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "received data from " << m_handle << ": " << buf << std::endl;
    m_buf += buf;
    return (g_reactor->registerEvent(m_handle, WRITE_EVENT));
}

int StreamHandler::handleWrite(void)
{
    if (write(m_handle, m_buf.c_str(), m_buf.size()) < 0)
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }
    m_buf.clear();
    return (g_reactor->unregisterEvent(m_handle, WRITE_EVENT));
}