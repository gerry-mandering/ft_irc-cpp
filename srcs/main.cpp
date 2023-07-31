#include "AcceptHandler.hpp"
#include "EnvManager.hpp"
#include "KqueueDemultiplexer.hpp"
#include "Reactor.hpp"
#include "def.h"
#include "parser.h"
#include <iostream>
#include <string>

#define PORT_LEN 5
#define PASSWORD_LEN 15

static bool invalid_input(const std::string &port, const std::string &password)
{
    if (port.length() > PORT_LEN || password.length() > PASSWORD_LEN)
        return (true);
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!std::isdigit(port[i]))
            return (true);
    }
    for (size_t i = 0; i < password.length(); i++)
    {
        if (!std::isalnum(password[i]))
            return (true);
    }
    return (false);
}

// TODO: 로깅 핸들러 추가 처리
static void init_server(const std::string &portStr, const std::string &password)
{
    int portInt = 0;

    for (size_t i = 0; i < portStr.length(); i++)
    {
        portInt *= 10;
        portInt += portStr[i] - '0';
    }
    Parser::initParsers();
    EnvManager::GetInstance()->SetPortNumber(portStr);
    EnvManager::GetInstance()->SetConnectionPassword(password);
    g_reactor().setDemultiplexer(new KqueueDemultiplexer());
    AcceptHandler *acceptHandler = new AcceptHandler(portInt, password);
    g_reactor().registerHandler(acceptHandler, READ_EVENT);
}

int main(int argc, char **argv)
{
    // TODO: singleton atexit 처리 고민할것
    if (argc != 3)
    {
        std::cerr << "Usage: <port> <password>\n";
        return (EXIT_FAILURE);
    }
    if (invalid_input(argv[1], argv[2]))
    {
        std::cerr << "Input is invalid\n"
                  << "port should be integer and password should be alphanumeric\n";
        return (EXIT_FAILURE);
    }
    try
    {
        init_server(argv[1], argv[2]);
    }
    catch (const std::exception &e)
    {
        // TODO: 에러 발생시 출력문, 종료여부 고민
        std::cerr << e.what() << "\n";
        return (EXIT_FAILURE);
    }
    while (true)
        g_reactor().handleEvents();
    return (0);
}