#ifndef EVAL_RPN_H
#define EVAL_RPN_H

#include <deque>

#include "c_token.h"
#include "result.h"

ResultAsString evalRpn(const std::deque<Token> &rpn);

#endif
