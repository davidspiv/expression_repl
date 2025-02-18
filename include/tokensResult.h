#ifndef TOKENRESULT_H
#define TOKENRESULT_H
#include <deque>
#include <string>

#include "token.h"

struct TokensResult {
  std::deque<Token> tokens;
  std::string errMessage;
};

#endif
