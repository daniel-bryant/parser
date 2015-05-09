%include
{
  #include <cassert>
  #include <iostream>
  #include "token.h"
}

%syntax_error
{
  *valid = false;
}

%token_type {Token*}
%type expr {Token}
%type arg {Token}
%extra_argument {bool* valid}

%nonassoc LOWEST .
%nonassoc DOT2 DOT3 .
%left PIPE CARET .
%left AMPER .
%left PLUS MINUS .
%left TIMES DIVIDE MODULO .
%right POW .

start ::= expr(A) .                  { /*if (A.str)
                                         std::cout << "variable: " << A.str << std::endl;
                                       if (A.num)
                                         std::cout << "result: " << A.num << std::endl;*/ }

expr(A) ::= arg .                  { A.num = 3; }

arg(A) ::= LPAREN arg(B) RPAREN .  { A.num = B.num; }
arg(A) ::= LPAREN RPAREN .         { A.num = 0; }

arg(A) ::= arg(B) DOT2 arg(C) .    { A.num = B.num + C.num; /*change*/ }
arg(A) ::= arg(B) DOT3 arg(C) .    { A.num = B.num + C.num; /*change*/ }
arg(A) ::= arg(B) PLUS arg(C) .    { A.num = B.num + C.num; }
arg(A) ::= arg(B) MINUS arg(C) .   { A.num = B.num - C.num; }
arg(A) ::= arg(B) TIMES arg(C) .   { A.num = B.num * C.num; }
arg(A) ::= arg(B) DIVIDE arg(C) .  { A.num = B.num / C.num; }
arg(A) ::= arg(B) MODULO arg(C) .  { A.num = B.num * C.num; /*change*/ }
arg(A) ::= arg(B) POW arg(C) .     { A.num = B.num * C.num; /*change*/ }
arg(A) ::= MINUS arg(B) . [LOWEST] { A.num = -B.num; }
arg(A) ::= arg PIPE arg .          { A.num = 2 | 3; }
arg(A) ::= arg CARET arg .         { A.num = 2 ^ 3; }
arg(A) ::= arg AMPER arg .         { A.num = 2 & 3; }

arg(A) ::= NUM(B) .                { A.num = B->num; }



arg ::= STRING .                   {}
arg(A) ::= IDENTIFIER(B) .         { A.str = B->str; }
arg(A) ::= PROPER_IDENTIFIER(B) .  { A.str = B->str; }
