#include "ParseException.hpp"
#include "parser.h"
#include "parser_internal.h"
#include <vector>

using namespace Parser;
using namespace std;

extern map<string, parser_t> parsers;

// Test한 함수: pass, nick, user, quit, topic, mode, join, part, privmsg

// TODO: privmsg 하드테스트, 인자 3개 이상 테스트
// :b\r\n 앞에 인자 개수 변경해가면서 테스트, 인자 앞에 접두사 #,:는 반드시 붙여볼 것
// 인자 0개, 인자 1개 ok, 2개 ok
static const char *commands[] = {"PASS #a :b\r\n",   "NICK #a :b\r\n", "USER #a :b\r\n",    "QUIT :bb\r\n",
                                 "TOPIC #a :b\r\n",  "MODE #a :b\r\n", "JOIN #a dddj\r\n",  "PART :a: :b :c :d\r\n",
                                 "INVITE #a :b\r\n", "KICK #a :b\r\n", "PRIVMSG #a :b\r\n", "PING #a :b\r\n",
                                 "PONG #a :b\r\n"};

void test_parseRequest(void)
{
    Request *request;
    vector<string> commandList;
    vector<string>::iterator it;
    const int size = sizeof(commands) / sizeof(commands[0]);
    int i = 0;

    for (i = 0; i < size; i++)
        commandList.push_back(commands[i]);

    i = 0;
    for (it = commandList.begin(); it != commandList.end(); it++)
    {
        try
        {
            request = parseRequest(*it, i);
        }
        catch (const ParseException &e)
        {
            std::cerr << e.what() << '\n';
        }
        i++;
    }
}

void test_parsePart(const string &str)
{
    Request *request;

    try
    {
        request = parseRequest(str + "\r\n", 0);
    }
    catch (const ParseException &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(void)
{
    Request *request;

    Parser::initParsers();
    test_parseRequest();

    // PART corner case
    cout << "\n---- PART corner case ----\n";
    test_parsePart("PART #a :b");
    test_parsePart("PART :a :b");
    test_parsePart("PART :a: :b");
    test_parsePart("PART :a:   :b  ");
    test_parsePart("PART :a:   :b :c :ddf :");

    // Privmsg corner case
    cout << "\n---- PART corner case ----\n";
    return (0);
}