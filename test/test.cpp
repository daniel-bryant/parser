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

  ExpectEQ(parse(std::string("alias identifierFoo identifierBar")), 1);

  ExpectEQ(parse(std::string("alias _fidFoo _fidBar")), 1);

  ExpectEQ(parse(std::string("alias CONSTANTFOO CONSTANTBAR")), 1);

  ExpectEQ(parse(std::string("alias identifierFoo _fidBar")), 1);

  ExpectEQ(parse(std::string("alias _fidFoo CONSTANTBAR")), 1);

  ExpectEQ(parse(std::string("alias CONSTANTFOO identifierBar")), 1);

  ExpectEQ(parse(std::string("alias | ^")), 1); // not testing all ops

  ExpectEQ(parse(std::string("alias & <=>")), 1); // not testing all ops

  ExpectEQ(parse(std::string("alias $global $global")), 1);

  ExpectEQ(parse(std::string("alias $globalFoo $globarBar")), 1);

  ExpectEQ(parse(std::string("alias $global $&")), 1);

  ExpectEQ(parse(std::string("alias $global $`")), 1);

  ExpectEQ(parse(std::string("alias $global $'")), 1);

  ExpectEQ(parse(std::string("alias $global $+")), 1);

  ExpectEQ(parse(std::string("alias $global $1")), 1);

  ExpectEQ(parse(std::string("alias $global $2")), 1);

  ExpectEQ(parse(std::string("alias $global $3")), 1);

  ExpectEQ(parse(std::string("alias $global $4")), 1);

  ExpectEQ(parse(std::string("alias $global $5")), 1);

  ExpectEQ(parse(std::string("alias $global $6")), 1);

  ExpectEQ(parse(std::string("alias $global $7")), 1);

  ExpectEQ(parse(std::string("alias $global $8")), 1);

  ExpectEQ(parse(std::string("alias $global $9")), 1);

  std::string reswords[21][2] = {
    {std::string("__LINE__"),     std::string("module")},
    {std::string("__FILE__"),     std::string("next")},
    {std::string("__ENCODING__"), std::string("nil")},
    {std::string("BEGIN"),        std::string("not")},
    {std::string("END"),          std::string("or")},
    {std::string("alias"),        std::string("redo")},
    {std::string("and"),          std::string("rescue")},
    {std::string("begin"),        std::string("retry")},
    {std::string("break"),        std::string("return")},
    {std::string("case"),         std::string("self")},
    {std::string("class"),        std::string("super")},
    {std::string("def"),          std::string("then")},
    {std::string("defined?"),     std::string("true")},
    {std::string("do"),           std::string("undef")},
    {std::string("else"),         std::string("when")},
    {std::string("elsif"),        std::string("yield")},
    {std::string("end"),          std::string("if")},
    {std::string("ensure"),       std::string("unless")},
    {std::string("false"),        std::string("while")},
    {std::string("for"),          std::string("until")},
    {std::string("in"),           std::string("until")},
  };
  std::string space = std::string(" ");

  // KEYWORD_alias fitem fitem; fitem ::= fsym; fsym ::= fname; fname ::= reswords
  std::string alias = std::string("alias");
  for (int i=0; i<21; ++i) {
    // ex. alias BEGIN END
    ExpectEQ(parse(alias + space + reswords[i][0] + space + reswords[i][1]), 1);
  }

  // KEYWORD_undef undef_list; undef_list ::= fitem; fitem ::= fsym; fsym ::= fname; fname ::= reswords
  std::string undef = std::string("undef");
  for (int i=0; i<21; i++) {
    // ex undef BEGIN
    ExpectEQ(parse(undef + space + reswords[i][0]), 1);
    ExpectEQ(parse(undef + space + reswords[i][1]), 1);
  }

  ExpectEQ(parse(std::string("undef identifierFoo")), 1);

  ExpectEQ(parse(std::string("undef identifierFoo, identifierBar")), 1);

  ExpectEQ(parse(std::string("undef identifierFoo, identifierBar, _fidFoo, CONSTANTFOO")), 1);

  ExpectEQ(parse(std::string("undef <, <=, >, >=, <<, >>, `, [], []=")), 1); // not testing all ops

  ExpectEQ(parse(std::string("2 + 2 _if_ 3 < 4")), 1);

  ExpectEQ(parse(std::string("2 + 2 _unless_ 1 == 1")), 1);

  ExpectEQ(parse(std::string("2 + 2 _while_ 100 >> 1")), 1);

  ExpectEQ(parse(std::string("2 + 2 _until_ 1 + 1")), 1);

  ExpectEQ(parse(std::string("2 + 2 _rescue_ 1 / 1")), 1);

  ExpectEQ(parse(std::string("END { 2 + 2 }")), 1);

  // stmt ::= lhs EQUALS mrhs
  ExpectEQ(parse(std::string("identifier = 1, 1")), 1);
  ExpectEQ(parse(std::string("nil = * 1 , 1")), 1);
  ExpectEQ(parse(std::string(":::Constant.identifier = * 1 , 1 , 1")), 1);
  ExpectEQ(parse(std::string(":::Constant::identifier = * 1 , * 1 , 1")), 1);
  ExpectEQ(parse(std::string(":::Constant.Constant = 1 , * 1")), 1);
  ExpectEQ(parse(std::string(":::Constant::Constant = * 1 , * 1")), 1);
  ExpectEQ(parse(std::string(":::Constant = 1 , 1 , * 1")), 1);
  ExpectEQ(parse(std::string(":::Constant = 1 , * 1 , * 1")), 1);
  ExpectEQ(parse(std::string("$& = * 1")), 1);

  // stmt ::= mlhs EQUALS mrhs_arg
  //
  //
  // mlhs ::= mlhs_basic
  //
  // mlhs_basic ::= mlhs_head
  // mlhs_item ::= mlhs_node
  ExpectEQ(parse(std::string("identifier , = 1")), 1);
  ExpectEQ(parse(std::string("identifier , identifier , = 1")), 1);
  // mlhs_item ::= tLPAREN mlhs_inner rparen
  ExpectEQ(parse(std::string("( identifier , ) = 1")), 1);
  ExpectEQ(parse(std::string("( identifier , identifier, ) = 1")), 1);
  //
  // mlhs ::= LPAREN mlhs_inner RPAREN
  ExpectEQ(parse(std::string("( ( identifier , identifier, ) ) = 1")), 1);

  ExpectEQ(parse(std::string("@instanceVar , = 1")), 1);
  ExpectEQ(parse(std::string("$globalVar , = 1")), 1);
  ExpectEQ(parse(std::string("CONSTANT , = 1")), 1);
  ExpectEQ(parse(std::string("@@classVar , = 1")), 1);
  ExpectEQ(parse(std::string("nil , = 1")), 1);
  ExpectEQ(parse(std::string("self , = 1")), 1);
  ExpectEQ(parse(std::string("true , = 1")), 1);
  ExpectEQ(parse(std::string("false , = 1")), 1);
  ExpectEQ(parse(std::string("__FILE__ , = 1")), 1);
  ExpectEQ(parse(std::string("__LINE__ , = 1")), 1);
  ExpectEQ(parse(std::string("__ENCODING__ , = 1")), 1);
  // mlhs_node ::= primary_value '[' opt_call_args rbracket
  ExpectEQ(parse(std::string(":::Constant.identifier , = 1")), 1);
  ExpectEQ(parse(std::string(":::Constant::identifier , = 1")), 1);
  ExpectEQ(parse(std::string(":::Constant.Constant , = 1")), 1);
  ExpectEQ(parse(std::string(":::Constant::Constant , = 1")), 1);
  ExpectEQ(parse(std::string(":::Constant , = 1")), 1);
  ExpectEQ(parse(std::string("$1 , = 1")), 1);
  ExpectEQ(parse(std::string("$& , = 1")), 1);
  //
  // mlhs_basic ::= mlhs_head mlhs_item
  ExpectEQ(parse(std::string("identifier , identifier = 1")), 1);
  ExpectEQ(parse(std::string("( identifier , identifier ) = 1")), 1);
  ExpectEQ(parse(std::string("( ( identifier , ) , identifier ) = 1")), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES mlhs_node
  ExpectEQ(parse(std::string("identifier , * identifier = 1")), 1);
  //ExpectEQ(parse(std::string(" ( identifier ) , * identifier = 1")), 1);
  ExpectEQ(parse(std::string("( identifier , ) , * identifier = 1")), 1);
  //ExpectEQ(parse(std::string(" ( identifier , ) * identifier = 1")), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES mlhs_node COMMA mlhs_post
  ExpectEQ(parse(std::string("identifier , * identifier , identifier = 1")), 1);
  ExpectEQ(parse(std::string("( identifier , ) , * identifier, identifier, identifier = 1")), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES
  ExpectEQ(parse(std::string("identifier , * = 1")), 1);
  ExpectEQ(parse(std::string("( identifier , ) , * = 1")), 1);
  //
  // mlhs_basic ::= mlhs_head TIMES COMMA mlhs_post
  ExpectEQ(parse(std::string("identifier , * , identifier = 1")), 1);
  ExpectEQ(parse(std::string("identifier , * , identifier , identifier = 1")), 1);
  //
  // mlhs_basic ::= TIMES mlhs_node
  ExpectEQ(parse(std::string("* identifier = 1")), 1);
  //
  // mlhs_basic ::= TIMES mlhs_node COMMA mlhs_post
  ExpectEQ(parse(std::string("* identifier , identifier = 1")), 1);
  ExpectEQ(parse(std::string("* identifier , identifier , identifier = 1")), 1);
  // mlhs_basic ::= TIMES
  ExpectEQ(parse(std::string("* = 1")), 1);
  // mlhs_basic ::= TIMES COMMA mlhs_post
  ExpectEQ(parse(std::string("* , identifier , identifier = 1")), 1);
  ExpectEQ(parse(std::string("* , identifier , identifier , identifier = 1")), 1);
  //
  //
  // mrhs_arg ::= mrhs
  //
  // mrhs ::= args COMMA arg_value
  ExpectEQ(parse(std::string("identifier , identifier = 1 , 1")), 1);
  // mrhs ::= args COMMA TIMES arg_value
  ExpectEQ(parse(std::string("identifier , identifier = 1 , * 1")), 1);
  // mrhs ::= TIMES arg_value
  ExpectEQ(parse(std::string("identifier , identifier = * 1")), 1);
  //
  // mrhs_arg ::= arg_value
  //
  // arg_value ::= arg
  ExpectEQ(parse(std::string("identifier , identifier = 1")), 1);

  // stmt ::= expr
  // expr ::= arg
  // arg ::= lhs EQUALS arg
  ExpectEQ(parse(std::string("identifierFoo = 1")), 1);

  ExpectEQ(parse(std::string("@instanceVarFoo = 1")), 1);

  ExpectEQ(parse(std::string("$globalVarFoo = 1")), 1);

  ExpectEQ(parse(std::string("ConstantFoo = 1")), 1);

  ExpectEQ(parse(std::string("@@classVarFoo = 1")), 1);

  ExpectEQ(parse(std::string("nil = 1")), 1);

  ExpectEQ(parse(std::string("self = 1")), 1);

  ExpectEQ(parse(std::string("true = 1")), 1);

  ExpectEQ(parse(std::string("false = 1")), 1);

  ExpectEQ(parse(std::string("__FILE__ = 1")), 1);

  ExpectEQ(parse(std::string("__LINE__ = 1")), 1);

  ExpectEQ(parse(std::string("__ENCODING__ = 1")), 1);

  ExpectEQ(parse(std::string("")), 1); // lhs ::= primary_value '[' opt_call_args rbracket TODO

  ExpectEQ(parse(std::string(":::CONSTANTFOO.identifierBar = 1")), 1); // lhs ::= primary_value DOT IDENTIFIER

  ExpectEQ(parse(std::string(":::CONSTANTFOO::identifierBar = 1")), 1); // lhs ::= primary_value COLON2 IDENTIFIER

  ExpectEQ(parse(std::string(":::CONSTANTFOO.CONSTANTBAR = 1")), 1); // lhs ::= primary_value DOT CONSTANT

  ExpectEQ(parse(std::string(":::CONSTANTFOO::CONSTANTBAR = 1")), 1); // lhs ::= primary_value COLON2 CONSTANT

  ExpectEQ(parse(std::string(":::CONSTANTFOO = 1")), 1); // lhs ::= COLON3 CONSTANT

  ExpectEQ(parse(std::string("$1 = 1")), 1); // lhs ::= backref // backref ::= NTH_REF

  ExpectEQ(parse(std::string("$& = 1")), 1); // lhs ::= backref // backref ::= BACK_REF

  ExpectEQ(parse(std::string("identifierFoo = 1 + 1")), 1);

  ExpectEQ(parse(std::string("identifierFoo = 1 _rescue_ 0")), 1); // stmt <= expr <= arg <= lhs EQUALS arg MODIFIER_RESCUE arg

  ExpectEQ(parse(std::string("identifierFoo = 1 _rescue_ 0 _rescue_ 0")), 1); // stmt MODIFIER_RESCUE stmt

  ExpectEQ(parse(std::string("identifierFoo += 1")), 1);

  ExpectEQ(parse(std::string("@instanceVarFoo -= 1")), 1);

  ExpectEQ(parse(std::string("$globalVarFoo *= 1")), 1);

  ExpectEQ(parse(std::string("ConstantFoo /= 1")), 1);

  ExpectEQ(parse(std::string("@@classVarFoo %= 1")), 1);

  ExpectEQ(parse(std::string("nil **= 1")), 1);

  ExpectEQ(parse(std::string("self += 1")), 1);

  ExpectEQ(parse(std::string("true -= 1")), 1);

  ExpectEQ(parse(std::string("false *= 1")), 1);

  ExpectEQ(parse(std::string("__FILE__ /= 1")), 1);

  ExpectEQ(parse(std::string("__LINE__ %= 1")), 1);

  ExpectEQ(parse(std::string("__ENCODING__ **= 1")), 1);

  ExpectEQ(parse(std::string("identifierFoo += 1 _rescue_ 0")), 1);

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

  /* lone identifiers and constants are not valid yet
  ExpectEQ(parse(std::string("a")), 1);

  ExpectEQ(parse(std::string("aaaaa")), 1);

  ExpectEQ(parse(std::string("foobar + 1")), 1);

  ExpectEQ(parse(std::string("F")), 1);

  ExpectEQ(parse(std::string("Foo")), 1);*/

  std::cout << "\n\nFinished." << std::endl;

  return 0;
}
