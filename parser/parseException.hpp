#pragma once

#include <exception>
#include <string>

class ParseException : public std::exception
{
  private:
    std::string _msg;
};

class NotEnoughParams : public ParseException
{
};