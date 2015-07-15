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

%nonassoc  MODIFIER_if MODIFIER_unless MODIFIER_while MODIFIER_until .
%nonassoc KEYWORD_defined .
%right EQUALS OP_ASGN .
%left MODIFIER_rescue .
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
top_stmt ::= KEYWORD_BEGIN LBRACE top_compstmt RBRACE .    {}

stmt ::= KEYWORD_alias fitem fitem .                       {}
stmt ::= KEYWORD_alias GVAR GVAR .                         {}
stmt ::= KEYWORD_alias GVAR BACK_REF .                     {}
stmt ::= KEYWORD_alias GVAR NTH_REF .                      {}
stmt ::= KEYWORD_undef undef_list .                        {}
stmt ::= stmt MODIFIER_if expr_value .                     {}
stmt ::= stmt MODIFIER_unless expr_value .                 {}
stmt ::= stmt MODIFIER_while expr_value .                  {}
stmt ::= stmt MODIFIER_until expr_value .                  {}
stmt ::= stmt MODIFIER_rescue stmt .                       {}
stmt ::= KEYWORD_END LBRACE top_compstmt RBRACE .          {}
stmt ::= lhs EQUALS mrhs .                                 {}
stmt ::= mlhs EQUALS mrhs_arg .                            {}
stmt ::= expr .                        {}

expr(A) ::= arg .                  { A.num = 3; }

expr_value ::= expr .              {}

/*
command ::= fcall command_args       %prec tLOWEST .{}
command ::= fcall command_args cmd_brace_block .{}
command ::= primary_value '.' operation2 command_args %prec tLOWEST .{}
command ::= primary_value '.' operation2 command_args cmd_brace_block .{}
command ::= primary_value tCOLON2 operation2 command_args %prec tLOWEST .{}
command ::= primary_value tCOLON2 operation2 command_args cmd_brace_block .{}
command ::= keyword_super command_args .{}
command ::= keyword_yield command_args .{}
command ::= keyword_return call_args .{}
command ::= keyword_break call_args .{}
command ::= keyword_next call_args .{}
*/

mlhs ::= mlhs_basic .               {}
mlhs ::= LPAREN mlhs_inner RPAREN . {}

mlhs_inner ::= mlhs_basic .               {}
mlhs_inner ::= LPAREN mlhs_inner RPAREN . {}

mlhs_basic ::= mlhs_head .                                  {}
mlhs_basic ::= mlhs_head mlhs_item .                        {}
mlhs_basic ::= mlhs_head TIMES mlhs_node .                  {}
mlhs_basic ::= mlhs_head TIMES mlhs_node COMMA mlhs_post .  {}
mlhs_basic ::= mlhs_head TIMES .                            {}
mlhs_basic ::= mlhs_head TIMES COMMA mlhs_post .            {}
mlhs_basic ::= TIMES mlhs_node .                            {}
mlhs_basic ::= TIMES mlhs_node COMMA mlhs_post .            {}
mlhs_basic ::= TIMES .                                      {}
mlhs_basic ::= TIMES COMMA mlhs_post .                      {}

mlhs_item ::= mlhs_node .                 {}
mlhs_item ::= LPAREN mlhs_inner RPAREN .  {}

mlhs_head ::= mlhs_item COMMA .           {}
mlhs_head ::= mlhs_head mlhs_item COMMA . {}

mlhs_post ::= mlhs_item .                 {}
mlhs_post ::= mlhs_post COMMA mlhs_item . {}

mlhs_node ::= user_variable .                                 {}
mlhs_node ::= keyword_variable .                              {}
mlhs_node ::= primary_value LBRACKET opt_call_args rbracket . {}
mlhs_node ::= primary_value DOT IDENTIFIER .                  {}
mlhs_node ::= primary_value COLON2 IDENTIFIER .               {}
mlhs_node ::= primary_value DOT CONSTANT .                    {}
mlhs_node ::= primary_value COLON2 CONSTANT .                 {}
mlhs_node ::= COLON3 CONSTANT .                               {}
mlhs_node ::= backref .                                       {}

lhs ::= user_variable .                                 {}
lhs ::= keyword_variable .                              {}
lhs ::= primary_value LBRACKET opt_call_args rbracket . {}
lhs ::= primary_value DOT IDENTIFIER .                  {}
lhs ::= primary_value COLON2 IDENTIFIER .               {}
lhs ::= primary_value DOT CONSTANT .                    {}
lhs ::= primary_value COLON2 CONSTANT .                 {}
lhs ::= COLON3 CONSTANT .                               {}
lhs ::= backref .                                       {}

fname ::= IDENTIFIER .             {}
fname ::= CONSTANT .               {}
fname ::= FID .                    {}
fname ::= op .                     {}
fname ::= reswords .               {}

fsym ::= fname .                   {}
/*fsym ::= symbol .                  {}*/

fitem ::= fsym .                   {}
/*fitem ::= dsym .                   {}*/

undef_list ::= fitem .                                     {}
undef_list ::= undef_list COMMA fitem .                    {}

op ::= PIPE .     {}
op ::= CARET .    {}
op ::= AMPER .    {}
op ::= CMP .      {}
op ::= EQ .       {}
op ::= EQQ .      {}
op ::= MATCH .    {}
op ::= NMATCH .   {}
op ::= GRT .      {}
op ::= GEQ .      {}
op ::= LST .      {}
op ::= LEQ .      {}
op ::= NEQ .      {}
op ::= LSHFT .    {}
op ::= RSHFT .    {}
op ::= PLUS .     {}
op ::= MINUS .    {}
op ::= TIMES .    {}
/*op ::= STAR .     {}*/
op ::= DIVIDE .   {}
op ::= MODULO .   {}
op ::= POW .      {}
/*op ::= DSTAR .    {}*/
op ::= BANG .     {}
op ::= TILDE .    {}
/*op ::= UPLUS .    {}*/
/*op ::= UMINUS .   {}*/
op ::= AREF .     {}
op ::= ASET .     {}
op ::= BACKTICK . {}

reswords ::= KEYWORD__LINE__ .      {}
reswords ::= KEYWORD__FILE__ .      {}
reswords ::= KEYWORD__ENCODING__ .  {}
reswords ::= KEYWORD_BEGIN .        {}
reswords ::= KEYWORD_END .          {}
reswords ::= KEYWORD_alias .        {}
reswords ::= KEYWORD_and .          {}
reswords ::= KEYWORD_begin .        {}
reswords ::= KEYWORD_break .        {}
reswords ::= KEYWORD_case .         {}
reswords ::= KEYWORD_class .        {}
reswords ::= KEYWORD_def .          {}
reswords ::= KEYWORD_defined .      {}
reswords ::= KEYWORD_do .           {}
reswords ::= KEYWORD_else .         {}
reswords ::= KEYWORD_elsif .        {}
reswords ::= KEYWORD_end .          {}
reswords ::= KEYWORD_ensure .       {}
reswords ::= KEYWORD_false .        {}
reswords ::= KEYWORD_for .          {}
reswords ::= KEYWORD_in .           {}
reswords ::= KEYWORD_module .       {}
reswords ::= KEYWORD_next .         {}
reswords ::= KEYWORD_nil .          {}
reswords ::= KEYWORD_not .          {}
reswords ::= KEYWORD_or .           {}
reswords ::= KEYWORD_redo .         {}
reswords ::= KEYWORD_rescue .       {}
reswords ::= KEYWORD_retry .        {}
reswords ::= KEYWORD_return .       {}
reswords ::= KEYWORD_self .         {}
reswords ::= KEYWORD_super .        {}
reswords ::= KEYWORD_then .         {}
reswords ::= KEYWORD_true .         {}
reswords ::= KEYWORD_undef .        {}
reswords ::= KEYWORD_when .         {}
reswords ::= KEYWORD_yield .        {}
reswords ::= KEYWORD_if .           {}
reswords ::= KEYWORD_unless .       {}
reswords ::= KEYWORD_while .        {}
reswords ::= KEYWORD_until .        {}

arg(A) ::= LPAREN arg(B) RPAREN .  { A.num = B.num; }
arg(A) ::= LPAREN RPAREN .         { A.num = 0; }

arg(A) ::= lhs EQUALS arg .        { A.num = 1; }
arg(A) ::= lhs EQUALS arg MODIFIER_rescue arg .      { A.num = 2; }
arg(A) ::= var_lhs OP_ASGN arg .                     { A.num = 3; }
arg(A) ::= var_lhs OP_ASGN arg MODIFIER_rescue arg . { A.num = 4; }
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
arg(A) ::= KEYWORD_defined opt_nl arg .        { A.num = 1; }
arg(A) ::= arg QUESTION arg opt_nl COLON arg . { A.num = 1; }

arg(A) ::= NUM(B) .                { A.num = B.num; A.str = B.str; }

arg_value ::= arg .                     {}

opt_call_args ::= none .  {}
opt_call_args ::= call_args .  {}
opt_call_args ::= args COMMA .  {}
opt_call_args ::= args COMMA assocs COMMA .  {}
opt_call_args ::= assocs COMMA .  {}

/*
call_args ::= command .                         {}
*/
call_args ::= args opt_block_arg .              {}
call_args ::= assocs opt_block_arg .            {}
call_args ::= args COMMA assocs opt_block_arg . {}
call_args ::= block_arg .                       {}

/*
command_args ::= call_args . {}
*/

block_arg ::= AMPER arg_value .  {}

opt_block_arg ::= COMMA block_arg . {}
opt_block_arg ::= none .            {}

args ::= arg_value .                    {}
args ::= TIMES arg_value .              {}
args ::= args COMMA arg_value .         {}
args ::= args COMMA TIMES arg_value .   {}

mrhs_arg ::= mrhs .       {}
mrhs_arg ::= arg_value .  {}

mrhs ::= args COMMA arg_value .         {}
mrhs ::= args COMMA TIMES arg_value .   {}
mrhs ::= TIMES arg_value .              {}

primary ::= COLON3 CONSTANT .      {}

primary_value ::= primary .        {}

user_variable ::= IDENTIFIER .     {}
user_variable ::= IVAR .           {}
user_variable ::= GVAR .           {}
user_variable ::= CONSTANT .       {}
user_variable ::= CVAR .           {}

keyword_variable ::= KEYWORD_nil .         {}
keyword_variable ::= KEYWORD_self .        {}
keyword_variable ::= KEYWORD_true .        {}
keyword_variable ::= KEYWORD_false .       {}
keyword_variable ::= KEYWORD__FILE__ .     {}
keyword_variable ::= KEYWORD__LINE__ .     {}
keyword_variable ::= KEYWORD__ENCODING__ . {}

var_lhs ::= user_variable .        {}
var_lhs ::= keyword_variable .     {}

backref ::= NTH_REF .              {}
backref ::= BACK_REF .             {}

assocs ::= assoc .              {}
assocs ::= assocs COMMA assoc . {}

assoc ::= arg_value ASSOC arg_value .                        {}
/*
assoc ::= tLABEL arg_value .                                  {}
assoc ::= tSTRING_BEG string_contents tLABEL_END arg_value .  {}
assoc ::= tDSTAR arg_value .                                  {}
*/

opt_terms ::= .                    {}
opt_terms ::= terms .              {}

opt_nl ::= .                       {}
opt_nl ::= NEWLINE .               {}

rbracket ::= opt_nl RBRACKET . {}

term ::= SEMICOLON .               {}
term ::= NEWLINE .                 {}

terms ::= term .                   {}
terms ::= terms SEMICOLON .        {}

none ::= .                         {}


arg ::= STRING .                   {}
