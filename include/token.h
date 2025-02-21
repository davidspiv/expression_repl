#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
 public:
  enum Type { Value, UnaryOp, BinaryOp, Parenthesis, Null };

 private:
  std::string symbol;
  Type type;

 public:
  Token() : symbol(""), type(Null) {};
  Token(const std::string &a, const Type &b) : symbol(a), type(b) {};
  ~Token() {};

  std::string getSymbol() const;
  Type getType() const;
  void setSymbol(const std::string &symbol);
  void setType(Type type);
  size_t length();
};

#endif
