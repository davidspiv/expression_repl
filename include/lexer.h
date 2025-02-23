#ifndef LEXER_H
#define LEXER_H
#include <deque>
#include <string>
#include <unordered_map>

#include "result.h"
#include "token.h"

ResultAsTokens lexer(const std::string &input);

const std::unordered_map<std::string, size_t> opRank = {
    {"(", 0},   {"+", 1},   {"-", 1},   {"*", 2},   {"/", 2},
    {"^", 3},   {"_", 4},   {"sin", 5}, {"cos", 5}, {"tan", 5},
    {"csc", 5}, {"sec", 5}, {"cot", 5}};

#endif
