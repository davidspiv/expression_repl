#ifndef EVAL_RPN_H
#define EVAL_RPN_H

#include <deque>

#include "result.h"
#include "token.h"

ResultAsString evalRpn(const std::deque<Token> &rpn);

#endif
