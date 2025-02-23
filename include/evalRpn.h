#ifndef EVAL_RPN_H
#define EVAL_RPN_H

#include <deque>

#include "result.h"
#include "token.h"

StringResult evalRpn(std::deque<Token> &rpnNotation);

#endif
