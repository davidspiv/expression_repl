#ifndef LEXER_H
#define LEXER_H
#include <deque>
#include <string>
#include <unordered_map>

#include "result.h"
#include "token.h"

ResultAsTokens lexer(const std::string &input);

#endif
