#include "StreamHandler.hpp"
#include "def.h"
#include <cstring>
#include <iostream>
#include <unistd.h>

StreamHandler::StreamHandler(handle_t handle) : m_handle(handle)
{
}

StreamHandler::~StreamHandler()
{
}

handle_t StreamHandler::getHandle(void) const
{
    return (m_handle);
}

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

    // std::string buffer;

    // // minseok2 구조 짜는중
    // std::string requestString = PacketManager::GetInstance()->HandlePacket(buffer);
    // string str = PacketManager::GetInstance->getRequest();

    // Request *request = Parser.Parse(requestString);

    // Validator *validator = Validator::GetInstance();
    // if (request->Accept(validator)) {
    //     Command *command = Command::GetCommand(request->GetType());
    //     request->Accept(command);
    // }

    return (g_reactor().registerEvent(this, WRITE_EVENT));
}

int StreamHandler::handleWrite(void)
{
    if (write(m_handle, m_buf.c_str(), m_buf.size()) < 0)
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }
    m_buf.clear();
    return (g_reactor().unregisterEvent(this, WRITE_EVENT));
}

int StreamHandler::handleError(void)
{
    return (0);
}