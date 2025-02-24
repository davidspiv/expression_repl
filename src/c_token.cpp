#include "../include/c_token.h"

#include <string>

std::string Token::getSymbol() const { return symbol; }

Token::Type Token::getType() const { return type; }

void Token::setSymbol(const std::string &symbol) { this->symbol = symbol; }

void Token::setType(Token::Type type) { this->type = type; }

size_t Token::length() { return symbol.length(); }
