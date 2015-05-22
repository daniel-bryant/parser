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

%token_type {Token}
%type expr {Token}
%type arg {Token}
%extra_argument {bool* valid}

%nonassoc LOWEST .

%nonassoc  MODIFIER_IF MODIFIER_UNLESS MODIFIER_WHILE MODIFIER_UNTIL .
%nonassoc KEYWORD_DEFINED .
%right EQUALS OP_ASGN .
%left MODIFIER_RESCUE .
%right QUESTION COLON .
%nonassoc DOT2 DOT3 .
%left OROP .
%left ANDOP .
%nonassoc  CMP EQ EQQ NEQ MATCH NMATCH .
%left  GRT GEQ LST LEQ .
%left PIPE CARET .
%left AMPER .
%left LSHFT RSHFT .
%left PLUS MINUS .
%left TIMES DIVIDE MODULO .
%right POW .
%right BANG TILDE .

program ::= top_compstmt .                                 {}

top_compstmt ::= top_stmts opt_terms .                     {}

top_stmts ::= none .                                       {}
top_stmts ::= top_stmt .                                   {}
top_stmts ::= top_stmts terms top_stmt .                   {}
/*top_stmts ::= error top_stmt . {}*/

top_stmt ::= stmt .                                        {}
top_stmt ::= KEYWORD_UP_BEGIN LBRACE top_compstmt RBRACE . {}

stmt ::= KEYWORD_ALIAS fitem fitem .                       {}
stmt ::= KEYWORD_ALIAS GVAR GVAR .                         {}
/*stmt ::= KEYWORD_ALIAS GVAR BACK_REF .                     {}*/
/*stmt ::= KEYWORD_ALIAS GVAR NTH_REF .                      {}*/
stmt ::= KEYWORD_UNDEF undef_list .                        {}
stmt ::= stmt MODIFIER_IF expr_value .                     {}
stmt ::= stmt MODIFIER_UNLESS expr_value .                 {}
stmt ::= stmt MODIFIER_WHILE expr_value .                  {}
stmt ::= stmt MODIFIER_UNTIL expr_value .                  {}
stmt ::= stmt MODIFIER_RESCUE stmt .                       {}
stmt ::= KEYWORD_UP_END LBRACE top_compstmt RBRACE .       {}
stmt ::= expr .                        {}

expr(A) ::= arg .                  { A.num = 3; }

expr_value ::= expr .              {}

lhs ::= user_variable .            {}
lhs ::= keyword_variable .         {}
/*lhs ::= primary_value '[' opt_call_args rbracket . {}*/
lhs ::= primary_value DOT IDENTIFIER .             {}
lhs ::= primary_value COLON2 IDENTIFIER .          {}
lhs ::= primary_value DOT CONSTANT .               {}
lhs ::= primary_value COLON2 CONSTANT .            {}
lhs ::= COLON3 CONSTANT .                          {}
/*lhs ::= backref .                                  {}*/

fname ::= IDENTIFIER .             {}
fname ::= CONSTANT .               {}
fname ::= FID .                    {}
/*fname ::= op .                     {}*/
/*fname ::= reswords .               {}*/

fsym ::= fname .                   {}
/*fsym ::= symbol .                  {}*/

fitem ::= fsym .                   {}
/*fitem ::= dsym .                   {}*/

undef_list ::= fitem .                                     {}
undef_list ::= undef_list COMMA fitem .                    {}

arg(A) ::= LPAREN arg(B) RPAREN .  { A.num = B.num; }
arg(A) ::= LPAREN RPAREN .         { A.num = 0; }

arg(A) ::= lhs EQUALS arg .        { A.num = 1; }
arg(A) ::= lhs EQUALS arg MODIFIER_RESCUE arg . { A.num = 2; }
arg(A) ::= var_lhs OP_ASGN arg .                     { A.num = 3; }
arg(A) ::= var_lhs OP_ASGN arg MODIFIER_RESCUE arg . { A.num = 4; }
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
arg(A) ::= arg CMP arg .           { A.num = 1; }
arg(A) ::= arg GRT arg .           { A.num = 1; }
arg(A) ::= arg GEQ arg .           { A.num = 1; }
arg(A) ::= arg LST arg .           { A.num = 1; }
arg(A) ::= arg LEQ arg .           { A.num = 1; }
arg(A) ::= arg EQ arg .            { A.num = 1; }
arg(A) ::= arg EQQ arg .           { A.num = 1; }
arg(A) ::= arg NEQ arg .           { A.num = 1; }
arg(A) ::= arg MATCH arg .         { A.num = 1; }
arg(A) ::= arg NMATCH arg .        { A.num = 1; }
arg(A) ::= BANG arg .              { A.num = 1; }
arg(A) ::= TILDE arg .             { A.num = 1; }
arg(A) ::= arg LSHFT arg .         { A.num = 1; }
arg(A) ::= arg RSHFT arg .         { A.num = 1; }
arg(A) ::= arg ANDOP arg .         { A.num = 1; }
arg(A) ::= arg OROP arg .          { A.num = 1; }
arg(A) ::= KEYWORD_DEFINED opt_nl arg .        { A.num = 1; }
arg(A) ::= arg QUESTION arg opt_nl COLON arg . { A.num = 1; }

arg(A) ::= NUM(B) .                { A.num = B.num; A.str = B.str; }

primary ::= COLON3 CONSTANT .      {}

primary_value ::= primary .        {}

user_variable ::= IDENTIFIER .     {}
user_variable ::= IVAR .           {}
user_variable ::= GVAR .           {}
user_variable ::= CONSTANT .       {}
user_variable ::= CVAR .           {}

keyword_variable ::= KEYWORD_NIL .         {}
keyword_variable ::= KEYWORD_SELF .        {}
keyword_variable ::= KEYWORD_TRUE .        {}
keyword_variable ::= KEYWORD_FALSE .       {}
keyword_variable ::= KEYWORD__FILE__ .     {}
keyword_variable ::= KEYWORD__LINE__ .     {}
keyword_variable ::= KEYWORD__ENCODING__ . {}

var_lhs ::= user_variable .        {}
var_lhs ::= keyword_variable .     {}

opt_terms ::= .                    {}
opt_terms ::= terms .              {}

opt_nl ::= .                       {}
opt_nl ::= NEWLINE .               {}

term ::= SEMICOLON .               {}
term ::= NEWLINE .                 {}

terms ::= term .                   {}
terms ::= terms SEMICOLON .        {}

none ::= .                         {}


arg ::= STRING .                   {}
