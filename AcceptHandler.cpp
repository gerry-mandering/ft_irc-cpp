#include "AcceptHandler.hpp"
#include "StreamHandler.hpp"
#include "def.h"
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

AcceptHandler::AcceptHandler(int port, std::string password)
    : EventHandler(-1), m_port(port), m_password(password)
{
}

AcceptHandler::~AcceptHandler() {}

// TODO: 예외처리 변경, 각 시스템콜 세부사항 공부
bool AcceptHandler::init()
{
    struct sockaddr_in addr;
    int opt = 1;

    m_handle = socket(AF_INET, SOCK_STREAM, 0);
    if (m_handle < 0)
    {
        perror("socket creation failed");
        return false;
    }
    if (setsockopt(m_handle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // h local ip에 바인
    addr.sin_port = htons(m_port);

    if (bind(m_handle, (sockaddr *)&addr, sizeof addr) == -1)
    {
        perror("bind failed");
        return false;
    }

    if (listen(m_handle, 5) == -1)
    {
        perror("listen failed");
        return false;
    }
    g_reactor().registerHandler(this, READ_EVENT);
    return true;
}

int AcceptHandler::handleRead(void)
{
    // IPv4, IPv6 둘다 호환되게 하기 위해 storage 사용
    struct sockaddr_storage addr;
    socklen_t addrSize = sizeof(addr);
    int newHandle;

    newHandle = accept(m_handle, (struct sockaddr *)&addr, &addrSize);
    // TODO: 예외처리 고민
    if (newHandle < 0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return (
        g_reactor().registerHandler(new StreamHandler(newHandle), READ_EVENT));
}

int AcceptHandler::handleWrite(void)
{
    return 0;
}

int AcceptHandler::handleError(void)
{
    return 0;
}