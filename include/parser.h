#ifndef PARSER_H
#define PARSER_H

#include <deque>

#include "result.h"
#include "token.h"

TokensResult parser(std::deque<Token> inputQueue);

#endif
