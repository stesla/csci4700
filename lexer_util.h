#ifndef LEXER_UTIL_H
#define LEXER_UTIL_H

#include "parser.h"

typedef int TOKEN;

void consume_comment(void);

TOKEN constant(YYSTYPE *yylval, const char *text);
TOKEN identifier(YYSTYPE *yylval, const char *text);
TOKEN string_literal(YYSTYPE *yylval, const char *text);

#endif LEXER_UTIL_H
