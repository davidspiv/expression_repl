#ifndef RESULT_H
#define RESULT_H
#include <deque>
#include <string>

#include "token.h"

struct TokensResult {
  std::deque<Token> tokens;
  std::string errMessage;
};

struct StringResult {
  std::string str;
  std::string errMessage;
};

#endif
