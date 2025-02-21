#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include <deque>

#include "result.h"
#include "token.h"

TokensResult shuntingYard(std::deque<Token> inputQueue);

#endif
