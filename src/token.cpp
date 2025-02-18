#include "../include/token.h"

#include <string>

std::string Token::getSymbol() { return symbol; }

Token::Type Token::getType() { return type; }

size_t Token::length() { return symbol.length(); }
