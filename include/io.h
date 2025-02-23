#ifndef IO_H
#define IO_H

#include <termios.h>

#include <string>

#include "result.h"

#define CLEAR "\033[0K"
#define GREY "\033[38;5;8m"
#define YELLOW "\033[33m"
#define WHITE "\033[0m"
#define PREV_LINE "\033[F"
#define CURSOR_LEFT "\033[1D"
#define CURSOR_RIGHT "\033[1C"

void setNonCanonicalMode(struct termios &initialSettings);
void restoreCanonicalMode(const struct termios &initialSettings);
bool readNextChar(char &ch);
void displayInput(const std::string &input, size_t cursorIndex);
void displayTempResult(const StringResult &result, size_t cursorIndex);
void displayError(const StringResult &result, size_t cursorIndex);
void displayResult(const std::string &result);

#endif
