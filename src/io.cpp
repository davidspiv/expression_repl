#ifdef _WIN32
#else
#include <termios.h>
#endif

#include <unistd.h>

#include <sstream>

#include "../include/_math.h"
#include "../include/expression.h"
#include "../include/io.h"
#include "../include/result.h"

#ifdef __linux__
void setNonCanonicalMode(struct termios &initialSettings) {
  struct termios newSettings;
  tcgetattr(STDIN_FILENO,
            &initialSettings);  // Get current terminal attributes
  newSettings = initialSettings;

  newSettings.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
  newSettings.c_cc[VMIN] = 1;               // Read one character at a time
  newSettings.c_cc[VTIME] = 0;              // No timeout

  tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);  // Apply new settings
}

void restoreCanonicalMode(const struct termios &initialSettings) {
  tcsetattr(STDIN_FILENO, TCSANOW,
            &initialSettings);  // Restore initial settings
}
#endif

bool readNextChar(char &ch) { return read(STDIN_FILENO, &ch, 1) == 1; }

bool isNumeric(const char symbol) { return isdigit(symbol) || symbol == '.'; }

bool isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) || opRank.count(std::string(1, ch)) ||
         ch == ')' || ch == ' ';
}

void updateDisplay(Expression &expression) {
  std::ostringstream out;

  if (!expression.getResult().empty() && expression.getErrMessage().empty()) {
    out << '\n'
        << CLEAR << GREY << stod(expression.getResult()) << WHITE << PREV_LINE;

    expression.isSecondLine = true;
  } else if (expression.isSecondLine) {
    out << '\n' << CLEAR << PREV_LINE;
  }

  out << '\r' << CLEAR << ">  " << expression.getInput();

  for (size_t i = 0;
       i < expression.getInput().length() - expression.getCursorIndex(); i++) {
    out << CURSOR_LEFT;
  }

  std::cout << out.str() << std::flush;
}

void displayResult(Expression &expression) {
  std::ostringstream out;

  if (expression.getErrMessage().size()) {
    out << '\n'
        << CLEAR << GREY << expression.getErrMessage() << WHITE << PREV_LINE;

    for (size_t i = 0; i < expression.getCursorIndex() + 3; i++) {
      out << CURSOR_RIGHT;
    }
  } else if (!expression.getResult().empty()) {
    out << '\n'
        << YELLOW << CLEAR << stod(expression.getResult()) << WHITE << '\n'
        << ">  ";
    expression.reset();
  }

  std::cout << out.str() << std::flush;
}
