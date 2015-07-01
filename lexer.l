%{
  #include "gram.hpp"
  #include "lexglobal.h"
  int line = 1, col = 1;
%}
 
%option reentrant
%option noyywrap
 
%x SINGLE_QUOTED
%x DOUBLE_QUOTED
 
%%

"."          {return DOT;}
".."         {return DOT2;}
"..."        {return DOT3;}
"+"          {return PLUS;}
"-"          {return MINUS;}
"*"          {return TIMES;}
"/"          {return DIVIDE;}
"%"          {return MODULO;}
"**"         {return POW;}
"+="         {return OP_ASGN;}
"-="         {return OP_ASGN;}
"*="         {return OP_ASGN;}
"/="         {return OP_ASGN;}
"%="         {return OP_ASGN;}
"**="        {return OP_ASGN;}
"|"          {return PIPE;}
"^"          {return CARET;}
"&"          {return AMPER;}
"<=>"        {return CMP;}
">"          {return GRT;}
">="         {return GEQ;}
"<"          {return LST;}
"<="         {return LEQ;}
"=="         {return EQ;}
"==="        {return EQQ;}
"!="         {return NEQ;}
"=~"         {return MATCH;}
"!~"         {return NMATCH;}
"!"          {return BANG;}
"~"          {return TILDE;}
"<<"         {return LSHFT;}
">>"         {return RSHFT;}
"&&"         {return ANDOP;}
"||"         {return OROP;}
"?"          {return QUESTION;}
":"          {return COLON;}
"::"         {return COLON2;}
":::"        {return COLON3;}
";"          {return SEMICOLON;}
"("          {return LPAREN;}
")"          {return RPAREN;}
"{"          {return LBRACE;}
"}"          {return RBRACE;}
","          {return COMMA;}
"="          {return EQUALS;}
"[]"         {return AREF;}
"[]="        {return ASET;}
"`"          {return BACKTICK;}
"$&"         {return BACK_REF;}
"$`"         {return BACK_REF;}
"$'"         {return BACK_REF;}
"$+"         {return BACK_REF;}
"$1"         {return NTH_REF;}
"$2"         {return NTH_REF;}
"$3"         {return NTH_REF;}
"$4"         {return NTH_REF;}
"$5"         {return NTH_REF;}
"$6"         {return NTH_REF;}
"$7"         {return NTH_REF;}
"$8"         {return NTH_REF;}
"$9"         {return NTH_REF;}

"__LINE__"      {return  KEYWORD__LINE__;}
"__FILE__"      {return  KEYWORD__FILE__;}
"__ENCODING__"  {return  KEYWORD__ENCODING__;}
"BEGIN"         {return  KEYWORD_BEGIN;}
"END"           {return  KEYWORD_END;}
"alias"         {return  KEYWORD_alias;}
"and"           {return  KEYWORD_and;}
"begin"         {return  KEYWORD_begin;}
"break"         {return  KEYWORD_break;}
"case"          {return  KEYWORD_case;}
"class"         {return  KEYWORD_class;}
"def"           {return  KEYWORD_def;}
"defined?"      {return  KEYWORD_defined;}
"do"            {return  KEYWORD_do;}
"else"          {return  KEYWORD_else;}
"elsif"         {return  KEYWORD_elsif;}
"end"           {return  KEYWORD_end;}
"ensure"        {return  KEYWORD_ensure;}
"false"         {return  KEYWORD_false;}
"for"           {return  KEYWORD_for;}
"in"            {return  KEYWORD_in;}
"module"        {return  KEYWORD_module;}
"next"          {return  KEYWORD_next;}
"nil"           {return  KEYWORD_nil;}
"not"           {return  KEYWORD_not;}
"or"            {return  KEYWORD_or;}
"redo"          {return  KEYWORD_redo;}
"rescue"        {return  KEYWORD_rescue;}
"retry"         {return  KEYWORD_retry;}
"return"        {return  KEYWORD_return;}
"self"          {return  KEYWORD_self;}
"super"         {return  KEYWORD_super;}
"then"          {return  KEYWORD_then;}
"true"          {return  KEYWORD_true;}
"undef"         {return  KEYWORD_undef;}
"when"          {return  KEYWORD_when;}
"yield"         {return  KEYWORD_yield;}
"if"            {return  KEYWORD_if;}
"unless"        {return  KEYWORD_unless;}
"while"         {return  KEYWORD_while;}
"until"         {return  KEYWORD_until;}
"_if_"          {return  MODIFIER_if;}
"_unless_"      {return  MODIFIER_if;}
"_while_"       {return  MODIFIER_if;}
"_until_"       {return  MODIFIER_if;}
"_rescue_"      {return  MODIFIER_if;}
 
[ \t\r]                 { col += (int) strlen(yytext); } /* ignore but count white space*/

\n                      { col = 0; ++line; return NEWLINE; }

[0-9]+|[0-9]*\.[0-9]+   { col += (int) strlen(yytext);
                          yylval.sval = strdup(yytext);
                          return NUM; }

[a-z][A-Za-z0-9]*       { col += (int) strlen(yytext);
                          yylval.sval = strdup(yytext);
                          return IDENTIFIER; }
 
_[a-z][A-Za-z0-9]*       { col += (int) strlen(yytext);
                          yylval.sval = strdup(yytext);
                          return FID; }

\$[a-z][A-Za-z0-9]*       { col += (int) strlen(yytext);
                          yylval.sval = strdup(yytext);
                          return GVAR; }

@@[a-z][A-Za-z0-9]*       { col += (int) strlen(yytext);
                          yylval.sval = strdup(yytext);
                          return CVAR; }

@[a-z][A-Za-z0-9]*       { col += (int) strlen(yytext);
                          yylval.sval = strdup(yytext);
                          return IVAR; }

[A-Z][A-Za-z0-9]*       { col += (int) strlen(yytext);
                          yylval.sval = strdup(yytext);
                          return CONSTANT; }

[']                     {BEGIN(SINGLE_QUOTED);}
<SINGLE_QUOTED>[^']+    {}
<SINGLE_QUOTED>[']      {BEGIN(INITIAL); return STRING;}
<SINGLE_QUOTED><<EOF>>  {return -1;}
 
["]                     {BEGIN(DOUBLE_QUOTED);}
<DOUBLE_QUOTED>[^"]+    {}
<DOUBLE_QUOTED>["]      {BEGIN(INITIAL); return STRING;}
<DOUBLE_QUOTED><<EOF>>  {return -1;}
 
.                       { col += (int) strlen(yytext); return yytext[0]; }
 
%%

/*
 * reset the line and column count
 */
void reset_lexer(void)
{
  line = 1;
  col  = 1;
}

/*
 * yyerror() is invoked when the lexer or the parser encounter
 * an error. The error message is passed via *s
 */
void yyerror(char *s)
{
  printf("error: %s at line: %d col: %d\n",s,line,col);
}
