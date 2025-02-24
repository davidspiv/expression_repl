#ifndef RESULT_H
#define RESULT_H
#include <deque>
#include <string>

#include "c_token.h"

struct ResultAsTokens {
  std::deque<Token> tokens;
  std::string errMessage;
};

struct ResultAsString {
  std::string str;
  std::string errMessage;
};

#endif
