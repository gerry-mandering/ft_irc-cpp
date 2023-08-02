#include "ParseException.hpp"
#include "color.h"
#include "parser.h"
#include "parser_internal.h"
#include <vector>

using namespace Parser;
using namespace std;

#define RESERT_CNT 1

extern map<string, parser_t> parsers;

// Test한 함수: pass, nick, user, quit, topic, join, part, privmsg

// :b\r\n 앞에 인자 개수 변경해가면서 테스트, 인자 앞에 접두사 #,:는 반드시 붙여볼 것
// 인자 0개, 인자 1개 ok, 2개 ok
static const char *commands[] = {"PASS #a :b\r\n",   "NICK #a :b\r\n", "USER #a :b\r\n",    "QUIT :bb\r\n",
                                 "TOPIC #a :b\r\n",  "MODE #a :b\r\n", "JOIN #a dddj\r\n",  "PART :a: :b :c :d\r\n",
                                 "INVITE #a :b\r\n", "KICK #a :b\r\n", "PRIVMSG #a :b\r\n", "PING #a :b\r\n",
                                 "PONG #a :b\r\n"};

static void test_parseRequest(void)
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

static void test_parseGeneral(const string &str, int cnt_reset = 0)
{
    Request *request;
    static int cnt = 1;

    if (cnt_reset)
    {
        cnt = 1;
        return;
    }
    cout << "test " << cnt++ << ": " << str + "\r\n"
         << "\n";
    try
    {
        request = parseRequest(str + "\r\n", 0);
    }
    catch (const ParseException &e)
    {
        std::cerr << e.what() << '\n';
    }
}

static void test_part()
{
    // PART corner case
    cout << "\n---- PART corner case ----\n";
    test_parseGeneral("PART #a :b");
    test_parseGeneral("PART #a \t :b");
    test_parseGeneral("PART #a\r\n :b");
    test_parseGeneral("PART #a :b\thi");
    test_parseGeneral("PART :a :b");
    test_parseGeneral("PART :a: :b");
    test_parseGeneral("PART :a:   :b  ");
    test_parseGeneral("PART :a:   :b :c :ddf :");
    test_parseGeneral("", RESERT_CNT);
}

static void test_sstream()
{
    // stringstream은 whitespace를 기준으로 토큰을 쪼갠다 (\r\n\t 전부 whitespace)
    // substring을 사용했기 때문에 마지막 \r\n이 남아있던 것이었음
    cout << "\n---- stringstream whitespace test ----\n";
    test_parseGeneral("PART #a \t :b");
    test_parseGeneral("PART #a\t :b");
    test_parseGeneral("PART #a \n :b");
    test_parseGeneral("PART #a\n :b");
    test_parseGeneral("PART #a\r\n :b");
    test_parseGeneral("PART #a\r\n\t \t :b");
    test_parseGeneral("PART #a :b\thi");
    test_parseGeneral("", RESERT_CNT);
}

static void test_quit()
{
    // QUIT corner case
    cout << "\n---- QUIT test ----\n";
    // 인자 1개
    test_parseGeneral("QUIT");
    test_parseGeneral("QUIT ");
    test_parseGeneral("QUIT \r\n");
    test_parseGeneral("QUIT \r\n  ");
    test_parseGeneral("QUIT \t");
    // 인자 2개
    test_parseGeneral("QUIT bbb");
    test_parseGeneral("QUIT dd sfjsd");
    test_parseGeneral("QUIT :b");
    test_parseGeneral("QUIT :b\r\n");
    test_parseGeneral("QUIT :b sfjlsdj \tsjdkfl\n jsd\n \n");
    test_parseGeneral("", RESERT_CNT);
}

static void test_privmsg()
{
    cout << "\n---- Privmsg test ----\n";
    // 기본 테스트
    // test_parseGeneral("PRIVMSG");
    // test_parseGeneral("PRIVMSG\n");
    // test_parseGeneral("PRIVMSG \t");
    // test_parseGeneral("PRIVMSG hi");
    // test_parseGeneral("PRIVMSG #hi");
    // test_parseGeneral("PRIVMSG #hi ");
    // test_parseGeneral("PRIVMSG\r\n");
    // test_parseGeneral("PRIVMSG #a :b");

    // 인자 3개 테스트
    // test_parseGeneral("PRIVMSG #a :b");
    // test_parseGeneral("PRIVMSG #a :b djfsk");
    // test_parseGeneral("PRIVMSG #a :b djfsk");
    // test_parseGeneral("PRIVMSG a :b djfsk");

    // 인자 여러개, 선행콜론 없을 때, 둘다 invalid 메시지로 처리
    // test_parseGeneral("PRIVMSG #a b");
    // test_parseGeneral("PRIVMSG #a b :c");

    // comma 테스트
    test_parseGeneral("PRIVMSG #a,b,#c,d :b,c");
    test_parseGeneral("PRIVMSG #acdc,bdfs,#c,dffsjd :b,c");
    test_parseGeneral("PRIVMSG #acdc, bdfs,#c,dffsjd :b,c");
    test_parseGeneral("PRIVMSG #acdc,bdfs,#c,dffsjd, :b,c");
    test_parseGeneral("PRIVMSG #acdc,bdfs,#c,dffsjd,,, :b,c");
    test_parseGeneral("PRIVMSG #acdc,,,,bdfs,,,#c,dffsjd,,, :b,c");
    test_parseGeneral("PRIVMSG ,,,,,#acdc,,,,bdfs,,,#c,dffsjd,,, :b,c");
}

static void test_kick()
{
    // 기본 테스트
    test_parseGeneral("KICK");
    test_parseGeneral("KICK user :b");
    test_parseGeneral("KICK #a :b");
    test_parseGeneral("KICK #a dah  :hihi");
    test_parseGeneral("KICK #a dah  hihi");
}

static void test_invite()
{
    // 기본 테스트
    test_parseGeneral("INVITE");
    test_parseGeneral("INVITE user :b");
    test_parseGeneral("INVITE #a :b");
    test_parseGeneral("INVITE #a dah  :hihi");
    test_parseGeneral("INVITE #a dah  hihi");
}

static void test_ping()
{
    test_parseGeneral("PING");
    test_parseGeneral("PING hihi");
    test_parseGeneral("PING hihi pong");
    test_parseGeneral("PING :hihi");
}

static void test_mode()
{
    // test_parseGeneral("MODE min +i");
    // test_parseGeneral("MODE min +o");
    // test_parseGeneral("MODE #ch +i");
    // test_parseGeneral("MODE #ch +i h");
    // test_parseGeneral("MODE #ch +t");
    // test_parseGeneral("MODE #ch +t h");
    // test_parseGeneral("MODE #ch -l h");
    // test_parseGeneral("MODE #ch -l");
    // test_parseGeneral("MODE ch -l");
}

static void test_topic()
{
    test_parseGeneral("TOPIC");
    test_parseGeneral("TOPIC #ch hi");
    test_parseGeneral("TOPIC #ch :hi");
    test_parseGeneral("TOPIC #ch :hia no no ");
    test_parseGeneral("TOPIC #ch :hia no no #");
    test_parseGeneral("TOPIC :ch :cha no no :");
}

int main(void)
{
    Request *request;

    Parser::initParsers();
    // test_parseRequest();

    // test_sstream();
    // test_part();
    // test_quit();
    // test_privmsg();
    // test_kick();
    // test_invite();
    // test_ping();
    test_mode();
    return (0);
}