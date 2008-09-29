#ifndef LEXER_UTIL_H
#define LEXER_UTIL_H

#include "parser.h"

typedef int TOKEN;

TOKEN constant(YYSTYPE *yylval, const char *text);
TOKEN identifier(YYSTYPE *yylval, const char *text);
TOKEN string_literal(YYSTYPE *yylval, const char *text);

void new_line(YYLTYPE *yylloc);

#endif
