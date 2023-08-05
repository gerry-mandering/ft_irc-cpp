#include "AcceptHandler.hpp"
#include "EnvManager.hpp"
#include "KqueueDemultiplexer.hpp"
#include "LoggingHandler.hpp"
#include "Reactor.hpp"
#include "parser.h"
#include <csignal>
#include <iostream>
#include <string>

#define PORT_LEN 5
#define PASSWORD_LEN 15
#define MAX_PORT 49151
#define MIN_PORT 1024

static int stringToint(const std::string &str)
{
    int ret = 0;

    for (size_t i = 0; i < str.length(); i++)
    {
        ret *= 10;
        ret += str[i] - '0';
    }
    return (ret);
}

static bool invalid_input(const std::string &port, const std::string &password)
{
    if (port.length() > PORT_LEN || password.length() > PASSWORD_LEN)
        return (true);
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!std::isdigit(port[i]))
            return (true);
    }
    if (stringToint(port) > MAX_PORT || stringToint(port) < MIN_PORT)
        return (true);
    for (size_t i = 0; i < password.length(); i++)
    {
        if (!std::isalnum(password[i]))
            return (true);
    }
    return (false);
}

static void init_server(const std::string &portStr, const std::string &password)
{
    int portInt = stringToint(portStr);

    Parser::initParsers();
    EnvManager::GetInstance()->SetPortNumber(portStr);
    EnvManager::GetInstance()->SetConnectionPassword(password);
    Reactor *reactor = Reactor::GetInstance();
    reactor->setDemultiplexer(new KqueueDemultiplexer());
    LoggingHandler *loggingHandler = new LoggingHandler();
    reactor->registerHandler(loggingHandler, OFF_EVENT);
    LOG_DEBUG("init logging handler");
    AcceptHandler *acceptHandler = new AcceptHandler(portInt, password);
    reactor->registerHandler(acceptHandler, READ_EVENT);
    LOG_DEBUG("init server success");
}

// TODO: 추후 삭제
// static void CheckLeaks()
//{
//    system("leaks ircserver");
//}
//
// static void signalHandler(int signum)
//{
//    if (signum == SIGINT)
//    {
//        CheckLeaks();
//        std::exit(EXIT_FAILURE);
//    }
//}

int main(int argc, char **argv)
{
    //    std::signal(SIGINT, signalHandler);
    if (argc != 3)
    {
        std::cerr << "Usage: <port> <password>\n";
        return (EXIT_FAILURE);
    }
    if (invalid_input(argv[1], argv[2]))
    {
        std::cerr << "Input is invalid\n"
                  << "Port should be integer inside a range and password should be alphanumeric\n";
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
    Reactor *reactor = Reactor::GetInstance();
    while (true)
    {
        if (reactor->handleEvents() < 0)
        {
            std::cerr << "Reactor kevent failed\n";
            return (EXIT_FAILURE);
        }
    }

    return (0);
}