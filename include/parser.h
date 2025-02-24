#ifndef PARSER_H
#define PARSER_H

#include <deque>

#include "c_token.h"
#include "result.h"

ResultAsTokens parser(std::deque<Token> inputQueue);

#endif
