#pragma once

#include <string>

namespace parser
{
void initParsers(void);
std::string codeToString(eInvaldFormat code);
std::string invalidFormatMsg(eInvaldFormat type, const std::string &msg);
void removeTrailingCRLF(std::string &str);
}