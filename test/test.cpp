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

  ExpectEQ(parse(""), 1);

  ExpectEQ(parse("\n"), 1);

  ExpectEQ(parse(";"), 1);

  ExpectEQ(parse("\n;"), 1);

  ExpectEQ(parse(";;;"), 1);

  ExpectEQ(parse("2..2;"), 1);

  ExpectEQ(parse("BEGIN { 2..2; }"), 1);

  ExpectEQ(parse("2..2;BEGIN { 2..2; }\n"), 1);

  ExpectEQ(parse("alias identifierFoo identifierBar"), 1);

  ExpectEQ(parse("alias _fidFoo _fidBar"), 1);

  ExpectEQ(parse("alias CONSTANTFOO CONSTANTBAR"), 1);

  ExpectEQ(parse("alias identifierFoo _fidBar"), 1);

  ExpectEQ(parse("alias _fidFoo CONSTANTBAR"), 1);

  ExpectEQ(parse("alias CONSTANTFOO identifierBar"), 1);

  ExpectEQ(parse("alias | ^"), 1); // not testing all ops

  ExpectEQ(parse("alias & <=>"), 1); // not testing all ops

  ExpectEQ(parse("alias $global $global"), 1);

  ExpectEQ(parse("alias $globalFoo $globarBar"), 1);

  ExpectEQ(parse("alias $global $&"), 1);

  ExpectEQ(parse("alias $global $`"), 1);

  ExpectEQ(parse("alias $global $'"), 1);

  ExpectEQ(parse("alias $global $+"), 1);

  ExpectEQ(parse("alias $global $1"), 1);

  ExpectEQ(parse("alias $global $2"), 1);

  ExpectEQ(parse("alias $global $3"), 1);

  ExpectEQ(parse("alias $global $4"), 1);

  ExpectEQ(parse("alias $global $5"), 1);

  ExpectEQ(parse("alias $global $6"), 1);

  ExpectEQ(parse("alias $global $7"), 1);

  ExpectEQ(parse("alias $global $8"), 1);

  ExpectEQ(parse("alias $global $9"), 1);

  std::string reswords[21][2] = {
    {"__LINE__",     "module"},
    {"__FILE__",     "next"},
    {"__ENCODING__", "nil"},
    {"BEGIN",        "not"},
    {"END",          "or"},
    {"alias",        "redo"},
    {"and",          "rescue"},
    {"begin",        "retry"},
    {"break",        "return"},
    {"case",         "self"},
    {"class",        "super"},
    {"def",          "then"},
    {"defined?",     "true"},
    {"do",           "undef"},
    {"else",         "when"},
    {"elsif",        "yield"},
    {"end",          "if"},
    {"ensure",       "unless"},
    {"false",        "while"},
    {"for",          "until"},
    {"in",           "until"},
  };
  std::string space = " ";
  std::string phrase;

  // KEYWORD_alias fitem fitem; fitem ::= fsym; fsym ::= fname; fname ::= reswords
  std::string alias = "alias";
  for (int i=0; i<21; ++i) {
    // ex. alias BEGIN END
    phrase = alias + space + reswords[i][0] + space + reswords[i][1];
    ExpectEQ(parse(phrase.c_str()), 1);
  }

  // KEYWORD_undef undef_list; undef_list ::= fitem; fitem ::= fsym; fsym ::= fname; fname ::= reswords
  std::string undef = "undef";
  for (int i=0; i<21; i++) {
    // ex undef BEGIN
    phrase = undef + space + reswords[i][0];
    ExpectEQ(parse(phrase.c_str()), 1);

    phrase = undef + space + reswords[i][1];
    ExpectEQ(parse(phrase.c_str()), 1);
  }

  ExpectEQ(parse("undef identifierFoo"), 1);

  ExpectEQ(parse("undef identifierFoo, identifierBar"), 1);

  ExpectEQ(parse("undef identifierFoo, identifierBar, _fidFoo, CONSTANTFOO"), 1);

  ExpectEQ(parse("undef <, <=, >, >=, <<, >>, `, [], []="), 1); // not testing all ops

  ExpectEQ(parse("2 + 2 _if_ 3 < 4"), 1);

  ExpectEQ(parse("2 + 2 _unless_ 1 == 1"), 1);

  ExpectEQ(parse("2 + 2 _while_ 100 >> 1"), 1);

  ExpectEQ(parse("2 + 2 _until_ 1 + 1"), 1);

  ExpectEQ(parse("2 + 2 _rescue_ 1 / 1"), 1);

  ExpectEQ(parse("END { 2 + 2 }"), 1);

  // stmt ::= lhs EQUALS mrhs
  ExpectEQ(parse("identifier = 1, 1"), 1);
  ExpectEQ(parse("nil = * 1 , 1"), 1);
  ExpectEQ(parse(":::Constant.identifier = * 1 , 1 , 1"), 1);
  ExpectEQ(parse(":::Constant::identifier = * 1 , * 1 , 1"), 1);
  ExpectEQ(parse(":::Constant.Constant = 1 , * 1"), 1);
  ExpectEQ(parse(":::Constant::Constant = * 1 , * 1"), 1);
  ExpectEQ(parse(":::Constant = 1 , 1 , * 1"), 1);
  ExpectEQ(parse(":::Constant = 1 , * 1 , * 1"), 1);
  ExpectEQ(parse("$& = * 1"), 1);

  // stmt ::= mlhs EQUALS mrhs_arg
  //
  //
  // mlhs ::= mlhs_basic
  //
  // mlhs_basic ::= mlhs_head
  // mlhs_item ::= mlhs_node
  ExpectEQ(parse("identifier , = 1"), 1);
  ExpectEQ(parse("identifier , identifier , = 1"), 1);
  // mlhs_item ::= tLPAREN mlhs_inner rparen
  ExpectEQ(parse("( identifier , ) = 1"), 1);
  ExpectEQ(parse("( identifier , identifier, ) = 1"), 1);
  //
  // mlhs ::= LPAREN mlhs_inner RPAREN
  ExpectEQ(parse("( ( identifier , identifier, ) ) = 1"), 1);

  ExpectEQ(parse("@instanceVar , = 1"), 1);
  ExpectEQ(parse("$globalVar , = 1"), 1);
  ExpectEQ(parse("CONSTANT , = 1"), 1);
  ExpectEQ(parse("@@classVar , = 1"), 1);
  ExpectEQ(parse("nil , = 1"), 1);
  ExpectEQ(parse("self , = 1"), 1);
  ExpectEQ(parse("true , = 1"), 1);
  ExpectEQ(parse("false , = 1"), 1);
  ExpectEQ(parse("__FILE__ , = 1"), 1);
  ExpectEQ(parse("__LINE__ , = 1"), 1);
  ExpectEQ(parse("__ENCODING__ , = 1"), 1);
  // mlhs_node ::= primary_value LBRACKET opt_call_args rbracket
  // opt_call_args ::= none
  ExpectEQ(parse(":::Constant [ ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ \n] , = 1"), 1);
  // opt_call_args ::= call_args
  //    call_args ::= command
  //    todo
  //    call_args ::= args opt_block_arg
  ExpectEQ(parse(":::Constant [ 1 , 2 ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 2 , & 1 ] , = 1"), 1);
  //    call_args ::= assocs opt_block_arg
  ExpectEQ(parse(":::Constant [ 1 => 1 , 2=>2 ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 => 1 , 2=>2 , & 1 ] , = 1"), 1);
  //    call_args ::= args COMMA assocs opt_block_arg
  ExpectEQ(parse(":::Constant [ 1 , 1 , 1=>1 ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 1 , 1=>1 , & 1 ] , = 1"), 1);
  //    call_args ::= block_arg
  ExpectEQ(parse(":::Constant [ & 1 ] , = 1"), 1);
  // opt_call_args ::= args COMMA
  ExpectEQ(parse(":::Constant [ 1 , ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ * 1 , ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 1 , ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , * 1 , ] , = 1"), 1);
  // opt_call_args ::= args COMMA assocs COMMA
  ExpectEQ(parse(":::Constant [ 1 , 1=>1, ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ * 1 , 1=>1, 2=>2, ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 1 , 1=>1, 2=>2, ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , * 1 , 1=>1, 2=>2, ] , = 1"), 1);
  // opt_call_args ::= assocs COMMA
  ExpectEQ(parse(":::Constant [ 1=>1, ] , = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1=>1, 2=>2, ] , = 1"), 1);
  // add more assocs when filled in
  //
  ExpectEQ(parse(":::Constant.identifier , = 1"), 1);
  ExpectEQ(parse(":::Constant::identifier , = 1"), 1);
  ExpectEQ(parse(":::Constant.Constant , = 1"), 1);
  ExpectEQ(parse(":::Constant::Constant , = 1"), 1);
  ExpectEQ(parse(":::Constant , = 1"), 1);
  ExpectEQ(parse("$1 , = 1"), 1);
  ExpectEQ(parse("$& , = 1"), 1);
  //
  // mlhs_basic ::= mlhs_head mlhs_item
  ExpectEQ(parse("identifier , identifier = 1"), 1);
  ExpectEQ(parse("( identifier , identifier ) = 1"), 1);
  ExpectEQ(parse("( ( identifier , ) , identifier ) = 1"), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES mlhs_node
  ExpectEQ(parse("identifier , * identifier = 1"), 1);
  //ExpectEQ(parse(" ( identifier ) , * identifier = 1"), 1);
  ExpectEQ(parse("( identifier , ) , * identifier = 1"), 1);
  //ExpectEQ(parse(" ( identifier , ) * identifier = 1"), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES mlhs_node COMMA mlhs_post
  ExpectEQ(parse("identifier , * identifier , identifier = 1"), 1);
  ExpectEQ(parse("( identifier , ) , * identifier, identifier, identifier = 1"), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES
  ExpectEQ(parse("identifier , * = 1"), 1);
  ExpectEQ(parse("( identifier , ) , * = 1"), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES COMMA mlhs_post
  ExpectEQ(parse("identifier , * , identifier = 1"), 1);
  ExpectEQ(parse("identifier , * , identifier , identifier = 1"), 1);
  //
  // mlhs_basic ::= TIMES mlhs_node
  ExpectEQ(parse("* identifier = 1"), 1);
  //
  // mlhs_basic ::= TIMES mlhs_node COMMA mlhs_post
  ExpectEQ(parse("* identifier , identifier = 1"), 1);
  ExpectEQ(parse("* identifier , identifier , identifier = 1"), 1);
  // mlhs_basic ::= TIMES
  ExpectEQ(parse("* = 1"), 1);
  // mlhs_basic ::= TIMES COMMA mlhs_post
  ExpectEQ(parse("* , identifier , identifier = 1"), 1);
  ExpectEQ(parse("* , identifier , identifier , identifier = 1"), 1);
  //
  //
  // mrhs_arg ::= mrhs
  //
  // mrhs ::= args COMMA arg_value
  ExpectEQ(parse("identifier , identifier = 1 , 1"), 1);
  // mrhs ::= args COMMA TIMES arg_value
  ExpectEQ(parse("identifier , identifier = 1 , * 1"), 1);
  // mrhs ::= TIMES arg_value
  ExpectEQ(parse("identifier , identifier = * 1"), 1);
  //
  // mrhs_arg ::= arg_value
  //
  // arg_value ::= arg
  ExpectEQ(parse("identifier , identifier = 1"), 1);

  // stmt ::= expr
  // expr ::= arg
  // arg ::= lhs EQUALS arg
  ExpectEQ(parse("identifierFoo = 1"), 1);

  ExpectEQ(parse("@instanceVarFoo = 1"), 1);

  ExpectEQ(parse("$globalVarFoo = 1"), 1);

  ExpectEQ(parse("ConstantFoo = 1"), 1);

  ExpectEQ(parse("@@classVarFoo = 1"), 1);

  ExpectEQ(parse("nil = 1"), 1);

  ExpectEQ(parse("self = 1"), 1);

  ExpectEQ(parse("true = 1"), 1);

  ExpectEQ(parse("false = 1"), 1);

  ExpectEQ(parse("__FILE__ = 1"), 1);

  ExpectEQ(parse("__LINE__ = 1"), 1);

  ExpectEQ(parse("__ENCODING__ = 1"), 1);

  // lhs ::= primary_value LBRACKET opt_call_args rbracket
  // opt_call_args ::= none
  ExpectEQ(parse(":::Constant [ ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ \n] = 1"), 1);
  // opt_call_args ::= call_args
  //    call_args ::= command
  //    todo
  //    call_args ::= args opt_block_arg
  ExpectEQ(parse(":::Constant [ 1 , 2 ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 2 , & 1 ] = 1"), 1);
  //    call_args ::= assocs opt_block_arg
  ExpectEQ(parse(":::Constant [ 1 => 1 , 2=>2 ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 => 1 , 2=>2 , & 1 ] = 1"), 1);
  //    call_args ::= args COMMA assocs opt_block_arg
  ExpectEQ(parse(":::Constant [ 1 , 1 , 1=>1 ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 1 , 1=>1 , & 1 ] = 1"), 1);
  //    call_args ::= block_arg
  ExpectEQ(parse(":::Constant [ & 1 ] = 1"), 1);
  // opt_call_args ::= args COMMA
  ExpectEQ(parse(":::Constant [ 1 , ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ * 1 , ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 1 , ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , * 1 , ] = 1"), 1);
  // opt_call_args ::= args COMMA assocs COMMA
  ExpectEQ(parse(":::Constant [ 1 , 1=>1, ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ * 1 , 1=>1, 2=>2, ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , 1 , 1=>1, 2=>2, ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1 , * 1 , 1=>1, 2=>2, ] = 1"), 1);
  // opt_call_args ::= assocs COMMA
  ExpectEQ(parse(":::Constant [ 1=>1, ] = 1"), 1);
  ExpectEQ(parse(":::Constant [ 1=>1, 2=>2, ] = 1"), 1);
  // add more assocs when filled in
  //

  ExpectEQ(parse(":::CONSTANTFOO.identifierBar = 1"), 1); // lhs ::= primary_value DOT IDENTIFIER

  ExpectEQ(parse(":::CONSTANTFOO::identifierBar = 1"), 1); // lhs ::= primary_value COLON2 IDENTIFIER

  ExpectEQ(parse(":::CONSTANTFOO.CONSTANTBAR = 1"), 1); // lhs ::= primary_value DOT CONSTANT

  ExpectEQ(parse(":::CONSTANTFOO::CONSTANTBAR = 1"), 1); // lhs ::= primary_value COLON2 CONSTANT

  ExpectEQ(parse(":::CONSTANTFOO = 1"), 1); // lhs ::= COLON3 CONSTANT

  ExpectEQ(parse("$1 = 1"), 1); // lhs ::= backref // backref ::= NTH_REF

  ExpectEQ(parse("$& = 1"), 1); // lhs ::= backref // backref ::= BACK_REF

  ExpectEQ(parse("identifierFoo = 1 + 1"), 1);

  ExpectEQ(parse("identifierFoo = 1 _rescue_ 0"), 1); // stmt <= expr <= arg <= lhs EQUALS arg MODIFIER_RESCUE arg

  ExpectEQ(parse("identifierFoo = 1 _rescue_ 0 _rescue_ 0"), 1); // stmt MODIFIER_RESCUE stmt

  ExpectEQ(parse("identifierFoo += 1"), 1);

  ExpectEQ(parse("@instanceVarFoo -= 1"), 1);

  ExpectEQ(parse("$globalVarFoo *= 1"), 1);

  ExpectEQ(parse("ConstantFoo /= 1"), 1);

  ExpectEQ(parse("@@classVarFoo %= 1"), 1);

  ExpectEQ(parse("nil **= 1"), 1);

  ExpectEQ(parse("self += 1"), 1);

  ExpectEQ(parse("true -= 1"), 1);

  ExpectEQ(parse("false *= 1"), 1);

  ExpectEQ(parse("__FILE__ /= 1"), 1);

  ExpectEQ(parse("__LINE__ %= 1"), 1);

  ExpectEQ(parse("__ENCODING__ **= 1"), 1);

  ExpectEQ(parse("identifierFoo += 1 _rescue_ 0"), 1);

  ExpectEQ(parse("2..2"), 1);

  ExpectEQ(parse("2...2"), 1);

  ExpectEQ(parse("2 + 2"), 1);

  ExpectEQ(parse("2 - 2"), 1);

  ExpectEQ(parse("2 * 2"), 1);

  ExpectEQ(parse("2 / 2"), 1);

  ExpectEQ(parse("2 % 2"), 1);

  ExpectEQ(parse("2 ** 2"), 1);

  ExpectEQ(parse("2 | 2"), 1);

  ExpectEQ(parse("2 ^ 2"), 1);

  ExpectEQ(parse("2 & 2"), 1);

  ExpectEQ(parse("2 <=> 2"), 1);

  ExpectEQ(parse("2 > 2"), 1);

  ExpectEQ(parse("2 >= 2"), 1);

  ExpectEQ(parse("2 < 2"), 1);

  ExpectEQ(parse("2 <= 2"), 1);

  ExpectEQ(parse("2 == 2"), 1);

  ExpectEQ(parse("2 === 2"), 1);

  ExpectEQ(parse("2 != 2"), 1);

  ExpectEQ(parse("2 =~ 2"), 1);

  ExpectEQ(parse("2 !~ 2"), 1);

  ExpectEQ(parse("! 2"), 1);

  ExpectEQ(parse("~ 2"), 1);

  ExpectEQ(parse("2 << 2"), 1);

  ExpectEQ(parse("2 >> 2"), 1);

  ExpectEQ(parse("2 && 2"), 1);

  ExpectEQ(parse("2 || 2"), 1);

  ExpectEQ(parse("defined? 2"), 1);

  ExpectEQ(parse("defined?\n2"), 1);

  ExpectEQ(parse("1 ? 2 : 3"), 1);

  ExpectEQ(parse("4 ? 5 \n : 6"), 1);

  ExpectEQ(parse("-2"), 1);

  ExpectEQ(parse("(2 + 2)"), 1);

  ExpectEQ(parse("2 + 2 + 2"), 1);

  ExpectEQ(parse("1 + 2 - 3 * 4 / 5"), 1);

  ExpectEQ(parse("1 * (2 + 3) / 4 + (5)"), 1);

  ExpectEQ(parse("()"), 1);

  /* lone identifiers and constants are not valid yet
  ExpectEQ(parse("a"), 1);

  ExpectEQ(parse("aaaaa"), 1);

  ExpectEQ(parse("foobar + 1"), 1);

  ExpectEQ(parse("F"), 1);

  ExpectEQ(parse("Foo"), 1);*/

  std::cout << "\n\nFinished." << std::endl;

  return 0;
}
