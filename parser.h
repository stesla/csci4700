#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

#define TRUE 1
#define FALSE 0

struct yystype {
  NODE *node;
  OP_TYPE op;
  char *text;
};

#ifdef  YYSTYPE
#undef  YYSTYPE
#endif
#define YYSTYPE struct yystype

NODE *semantic_analysis(const char *file, int lexer_debug, int parser_debug);

#endif PARSER_H
