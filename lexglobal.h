#ifndef YYSTYPE
typedef union {
  const char *sval;
  struct symtab *symp;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
