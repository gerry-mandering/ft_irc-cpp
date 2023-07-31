#include "AcceptHandler.hpp"
#include "LoggingHandler.hpp"
#include "StreamHandler.hpp"
#include "def.h"
#include "wrapper.h"
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

AcceptHandler::AcceptHandler(int port, std::string password)
    : EventHandler(), m_handle(-1), m_port(port), m_password(password)
{
    struct sockaddr_in addr;
    int opt = 1;

    m_handle = Wrapper::socket(AF_INET, SOCK_STREAM, 0);
    fcntl(m_handle, F_SETFL, O_NONBLOCK);
    Wrapper::setsockopt(m_handle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // h local ip에 바인
    addr.sin_port = htons(m_port);
    Wrapper::bind(m_handle, (sockaddr *)&addr, sizeof(addr));
    Wrapper::listen(m_handle, BACK_LOG);
}

AcceptHandler::~AcceptHandler() {}

handle_t AcceptHandler::getHandle(void) const
{
    return (m_handle);
}

int AcceptHandler::handleRead(void)
{
    // IPv4, IPv6 둘다 호환되게 하기 위해 storage 사용
    struct sockaddr_storage addr;
    socklen_t addrSize = sizeof(addr);
    int newHandle;

    newHandle = Wrapper::accept(m_handle, (struct sockaddr *)&addr, &addrSize);
    LOG_TRACE("new connection: " << newHandle);
    fcntl(newHandle, F_SETFL, O_NONBLOCK);
    return (g_reactor().registerHandler(new StreamHandler(newHandle), READ_EVENT));
}

int AcceptHandler::handleWrite(void)
{
    return 0;
}

int AcceptHandler::handleError(void)
{
    return 0;
}