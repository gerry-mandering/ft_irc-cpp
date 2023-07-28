#pragma once

#include <map>
#include <string>

class Request;

Request *parsePass(const std::string &tcpStream);
Request *parseNick(const std::string &tcpStream);
Request *parseUser(const std::string &tcpStream);
Request *parseQuit(const std::string &tcpStream);
Request *parseTopic(const std::string &tcpStream);
Request *parseMode(const std::string &tcpStream);
Request *parseJoin(const std::string &tcpStream);
Request *parsePart(const std::string &tcpStream);
Request *parseInvite(const std::string &tcpStream);
Request *parseKick(const std::string &tcpStream);
Request *parsePrivmsg(const std::string &tcpStream);
Request *parsePing(const std::string &tcpStream);
Request *parsePong(const std::string &tcpStream);
bool isalnum(const std::string &str);
bool isalpha(const std::string &str);

typedef Request *(*parser_t)(const std::string &tcpStream);
