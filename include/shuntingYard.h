#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include <deque>

#include "result.h"
#include "token.h"

TokensResult shuntingYard(std::deque<Token> inputQueue);

#endif
