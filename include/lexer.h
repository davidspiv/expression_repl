#ifndef LEXER_H
#define LEXER_H
#include <deque>
#include <string>
#include <unordered_map>

#include "c_token.h"
#include "result.h"

ResultAsTokens lexer(const std::string &input);

#endif
