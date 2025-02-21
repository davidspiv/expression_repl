#ifndef EVALRPNNOTATION_H
#define EVALRPNNOTATION_H

#include <deque>

#include "result.h"
#include "token.h"

StringResult evalRpnNotation(std::deque<Token> &rpnNotation);

#endif
