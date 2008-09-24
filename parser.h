#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

#define TRUE 1
#define FALSE 0

struct yystype {
  char *text;
  NODE *node;
};

#ifdef  YYSTYPE
#undef  YYSTYPE
#endif
#define YYSTYPE struct yystype

#endif PARSER_H
