#include <cstdlib>
#include <iostream>
#include <string>
#include "../parse.h"

int main() {
  std::string commandLine = std::string("2 + 2");
  parse(commandLine);
  commandLine = std::string("2 ++++");
  parse(commandLine);
  return 0;
}
