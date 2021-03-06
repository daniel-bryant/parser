#include "parse.h"

#include <iostream> // for std::cerr
#include "gram.hpp"
#include "lexer.yy.hpp"
#include "lexglobal.h"
#include "token.h"

void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void* parser, int token, Token tokenInfo, bool* valid);
void ParseFree(void* parser, void(*freeProc)(void*));
YYSTYPE yylval;

int parse(const char* str) {
  int retval = 1;
  // Set up the scanner
  yyscan_t scanner;
  yylex_init(&scanner);
  YY_BUFFER_STATE bufferState = yy_scan_string(str, scanner);

  // Set up the parser
  void* gramParser = ParseAlloc(malloc);

  int lexCode;
  struct Token tokenInfo;
  bool validParse = true;
  do {
    lexCode = yylex(scanner);
    tokenInfo.str = yylval.sval;
    tokenInfo.num = 123; // will be removed
    Parse(gramParser, lexCode, tokenInfo, &validParse);
  }
  while (lexCode > 0 && validParse);

  if (-1 == lexCode) {
    retval = -1;
    std::cerr << "The scanner encountered an error.\n";
  }

  if (!validParse) {
    retval = -1;
    std::cerr << "The parser encountered an error.\n";
  }

  // Cleanup the scanner and parser
  yy_delete_buffer(bufferState, scanner);
  yylex_destroy(scanner);
  ParseFree(gramParser, free);

  return retval;
}
