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

".."         {return DOT2;}
"..."        {return DOT3;}
"+"          {return PLUS;}
"-"          {return MINUS;}
"*"          {return TIMES;}
"/"          {return DIVIDE;}
"%"          {return MODULO;}
"**"         {return POW;}
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
"alias"      {return KEYWORD_ALIAS;}
"BEGIN"      {return KEYWORD_UP_BEGIN;}
"defined?"   {return KEYWORD_DEFINED;}
"undef"      {return KEYWORD_UNDEF;}
"?"          {return QUESTION;}
":"          {return COLON;}
";"          {return SEMICOLON;}
"("          {return LPAREN;}
")"          {return RPAREN;}
"{"          {return LBRACE;}
"}"          {return RBRACE;}
","          {return COMMA;}
 
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
