#ifndef LEXER_UTIL_H
#define LEXER_UTIL_H

typedef int TOKEN;

void consume_comment(void);

TOKEN constant(const char *text);
TOKEN identifier(const char *text);
TOKEN string_literal(const char *text);

#endif LEXER_UTIL_H
