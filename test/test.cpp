#include <cstdlib>
#include <iostream>
#include <string>
#include "../parse.h"

void ExpectEQ(int actual, int expected) {
  if (actual == expected) {
    std::cout << "\033[1;32m.\033[0m";
  } else {
    std::cout << "\033[1;31mx\033[0m";
  }
}

int main() {
  std::cout << "Running tests...\n" << std::endl;

  ExpectEQ(parse(std::string("")), 1);

  ExpectEQ(parse(std::string("\n")), 1);

  ExpectEQ(parse(std::string(";")), 1);

  ExpectEQ(parse(std::string("\n;")), 1);

  ExpectEQ(parse(std::string(";;;")), 1);

  ExpectEQ(parse(std::string("2..2;")), 1);

  ExpectEQ(parse(std::string("BEGIN { 2..2; }")), 1);

  ExpectEQ(parse(std::string("2..2;BEGIN { 2..2; }\n")), 1);

  ExpectEQ(parse(std::string("2..2")), 1);

  ExpectEQ(parse(std::string("2...2")), 1);

  ExpectEQ(parse(std::string("2 + 2")), 1);

  ExpectEQ(parse(std::string("2 - 2")), 1);

  ExpectEQ(parse(std::string("2 * 2")), 1);

  ExpectEQ(parse(std::string("2 / 2")), 1);

  ExpectEQ(parse(std::string("2 % 2")), 1);

  ExpectEQ(parse(std::string("2 ** 2")), 1);

  ExpectEQ(parse(std::string("2 | 2")), 1);

  ExpectEQ(parse(std::string("2 ^ 2")), 1);

  ExpectEQ(parse(std::string("2 & 2")), 1);

  ExpectEQ(parse(std::string("2 <=> 2")), 1);

  ExpectEQ(parse(std::string("2 > 2")), 1);

  ExpectEQ(parse(std::string("2 >= 2")), 1);

  ExpectEQ(parse(std::string("2 < 2")), 1);

  ExpectEQ(parse(std::string("2 <= 2")), 1);

  ExpectEQ(parse(std::string("2 == 2")), 1);

  ExpectEQ(parse(std::string("2 === 2")), 1);

  ExpectEQ(parse(std::string("2 != 2")), 1);

  ExpectEQ(parse(std::string("2 =~ 2")), 1);

  ExpectEQ(parse(std::string("2 !~ 2")), 1);

  ExpectEQ(parse(std::string("! 2")), 1);

  ExpectEQ(parse(std::string("~ 2")), 1);

  ExpectEQ(parse(std::string("2 << 2")), 1);

  ExpectEQ(parse(std::string("2 >> 2")), 1);

  ExpectEQ(parse(std::string("2 && 2")), 1);

  ExpectEQ(parse(std::string("2 || 2")), 1);

  ExpectEQ(parse(std::string("defined? 2")), 1);

  ExpectEQ(parse(std::string("defined?\n2")), 1);

  ExpectEQ(parse(std::string("1 ? 2 : 3")), 1);

  ExpectEQ(parse(std::string("4 ? 5 \n : 6")), 1);

  ExpectEQ(parse(std::string("-2")), 1);

  ExpectEQ(parse(std::string("(2 + 2)")), 1);

  ExpectEQ(parse(std::string("2 + 2 + 2")), 1);

  ExpectEQ(parse(std::string("1 + 2 - 3 * 4 / 5")), 1);

  ExpectEQ(parse(std::string("1 * (2 + 3) / 4 + (5)")), 1);

  ExpectEQ(parse(std::string("()")), 1);

  ExpectEQ(parse(std::string("a")), 1);

  ExpectEQ(parse(std::string("aaaaa")), 1);

  ExpectEQ(parse(std::string("foobar + 1")), 1);

  ExpectEQ(parse(std::string("F")), 1);

  ExpectEQ(parse(std::string("Foo")), 1);

  std::cout << "\n\nFinished." << std::endl;

  return 0;
}
