#include <cstdlib>
#include <iostream>
#include <string>
#include "parse.h"

int main() {
  std::string commandLine;
  while (getline(std::cin, commandLine)) {
    parse(commandLine.c_str());
  }
  return 0;
}
