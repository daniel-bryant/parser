#include "parse.h"

#include <iostream> // for std::cerr
#include "gram.hpp"
#include "lexer.yy.hpp"
#include "lexglobal.h"
#include "token.h"

void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void* parser, int token, Token* tokenInfo, bool* valid);
void ParseFree(void* parser, void(*freeProc)(void*));
YYSTYPE yylval;

void parse(const std::string& commandLine) {
  // Set up the scanner
  yyscan_t scanner;
  yylex_init(&scanner);
  YY_BUFFER_STATE bufferState = yy_scan_string(commandLine.c_str(), scanner);

  // Set up the parser
  void* gramParser = ParseAlloc(malloc);

  int lexCode;
  struct Token tokenInfo;
  bool validParse = true;
  do {
    lexCode = yylex(scanner);
    //cout << "lexCode: " << lexCode << ", yylval: " << yylval.dval << endl;
    tokenInfo.num = yylval.dval;
    tokenInfo.str = yylval.sval;
    Parse(gramParser, lexCode, &tokenInfo, &validParse);
  }
  while (lexCode > 0 && validParse);

  if (-1 == lexCode) {
    std::cerr << "The scanner encountered an error.\n";
  }

  if (!validParse) {
    std::cerr << "The parser encountered an error.\n";
  }

  // Cleanup the scanner and parser
  yy_delete_buffer(bufferState, scanner);
  yylex_destroy(scanner);
  ParseFree(gramParser, free);
}
