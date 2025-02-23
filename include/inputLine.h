#ifndef INPUT_LINE_H
#define INPUT_LINE_H

#include <string>

class InputLine {
 private:
  size_t cursorIndex;
  std::string text;

 public:
  InputLine() : cursorIndex(0), text("") {};
  ~InputLine() {};
  size_t getCursorIndex() const;
  std::string getText() const;
  void setText(const std::string &text);
  void setCursorIndex(const size_t cursorIndex);
  void erase();
  void insert(char ch);
  void reset();
  void operator++();
  void operator--();
};

#endif
