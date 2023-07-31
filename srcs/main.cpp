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
    }
    EnvManager::GetInstance()->SetPortNumber(std::string(argv[1]));
    EnvManager::GetInstance()->SetConnectionPassword(std::string(argv[2]));

    Parser::initParsers();

    // TODO AcceptHandler 내부에서 불러오는 방식으로 수정?
    AcceptHandler acceptHandler(PORT, PASSWORD);
    g_reactor().setDemultiplexer(new KqueueDemultiplexer());

    if (!acceptHandler.init())
    {
        std::cerr << "Failed to initialize accept handler." << std::endl;
        return (1);
    }
    while (true)
        g_reactor().handleEvents();
    return (0);
}