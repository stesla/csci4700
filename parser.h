#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

struct yystype {
  NODE *node;
  OP_TYPE op;
  char *text;
};

#ifdef YYSTYPE
#undef YYSTYPE
#endif
#define YYSTYPE struct yystype

struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};

#ifdef YYLTYPE
#undef YYLTYPE
#endif
#define YYLTYPE struct yyltype
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1

NODE *semantic_analysis(const char *file, int lexer_debug, int parser_debug);

#endif
