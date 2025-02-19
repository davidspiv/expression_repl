#include "../include/io.h"

#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
using namespace std;

bool isSecondLine = false;

void setNonCanonicalMode(struct termios &initialSettings) {
  struct termios newSettings;
  tcgetattr(STDIN_FILENO, &initialSettings);  // Get current terminal attributes
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

bool readNextChar(char &ch) { return read(STDIN_FILENO, &ch, 1) == 1; }

void displayInput(const string &input, const string &result,
                  size_t cursorIndex) {
  ostringstream out;

  if (!result.empty()) {
    out << '\n' << CLEAR << GREY << stod(result) << WHITE << PREV_LINE;

    isSecondLine = true;

  } else if (isSecondLine) {
    out << '\n' << CLEAR << PREV_LINE;
  }

  out << '\r' << CLEAR << ">  " << input;

  for (size_t i = 0; i < input.size() - cursorIndex; i++) {
    out << CURSOR_LEFT;
  }

  cout << out.str() << flush;
}

void displayResult(const string &result) {
  if (!result.empty()) {
    cout << '\n'
         << YELLOW << CLEAR << stod(result) << WHITE << '\n'
         << ">  " << flush;
  }
}
