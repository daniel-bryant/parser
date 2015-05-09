%include
{
  #include <cassert>
  #include <iostream>
  #include <math.h>
  #include "token.h"
}

%syntax_error
{
  *valid = false;
}

%token_type {Token*}
%type expr {Token}
%extra_argument {bool* valid}

%left PLUS MINUS .
%left TIMES DIVIDE MODULO .
%right POW .
%right LOWEST .

start ::= expr(A) .                  { /*if (A.str)
                                         std::cout << "variable: " << A.str << std::endl;
                                       if (A.num)
                                         std::cout << "result: " << A.num << std::endl;*/ }

expr(A) ::= expr(B) PLUS expr(C) .   { A.num = B.num + C.num; }
expr(A) ::= expr(B) MINUS expr(C) .  { A.num = B.num - C.num; }
expr(A) ::= expr(B) TIMES expr(C) .  { A.num = B.num * C.num; }
expr(A) ::= expr(B) DIVIDE expr(C) . { if (C.num != 0)
                                         A.num = B.num / C.num;
                                       else
                                         std::cerr << "Error: divide by zero!" << std::endl; }
expr(A) ::= expr MODULO expr .       { A.num = 5; }
expr(A) ::= expr POW expr .          { A.num = 6; }
expr(A) ::= MINUS expr(B) . [LOWEST] { A.num = -B.num; }

expr(A) ::= LPAREN expr(B) RPAREN .  { A.num = B.num; }
expr(A) ::= LPAREN RPAREN .          { A.num = 0; }

expr(A) ::= NUM(B) .                 { A.num = B->num; }
expr ::= STRING .                    {}
expr(A) ::= IDENTIFIER(B) .          { A.str = B->str; }
expr(A) ::= PROPER_IDENTIFIER(B) .   { A.str = B->str; }
