#pragma once

#include <string>

namespace Parser
{
void initParsers(void);
std::string eTypeToString(Parser::eInvaldFormat code);
std::string invalidFormatMsg(Parser::eInvaldFormat type, const std::string &msg);
void removeTrailingCRLF(std::string &str);
bool isalnum(const std::string &str);
bool hasMetaChar(const std::string &str);
// TODO: isalph 필요성 여부
// bool isalpha(const std::string &str);
}