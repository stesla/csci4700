#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer_util.h"
#include "y.tab.h"

struct tab {
  char word[ 9 ];
  int value;
};

static int compare(const void *a, const void *b)
{
  return(strcmp(((struct tab *) a)->word,
                ((struct tab *) b)->word));
}

static void set_text(YYSTYPE *yylval, const char *text)
{
  if ((yylval->text = strdup(text)) == NULL)
    {
      perror("strdup");
      exit(1);
    }
}

TOKEN constant(YYSTYPE *yylval, const char *text)
{
  set_text(yylval, text);
  return CONSTANT;
}

TOKEN identifier(YYSTYPE *yylval, const char *text)
{
  /* Not everything that matches the identifier regex is an identifier. Some
   * are actually keywords. So we need to return the correct token for
   * those. Furthermore, there is no reason to set yylval.text for those. */
  struct tab *ptr;
  static struct tab table[] = {
    { "array", ARRAY },
    { "else", ELSE },
    { "for", FOR },
    { "global", GLOBAL },
    { "if", IF },
    { "read", READ },
    { "return", RETURN },
    { "while", WHILE },
    { "write", WRITE }
  };

  ptr = (struct tab *) bsearch(text,
                               (char *) &table[0],
                               sizeof(table) / sizeof(table[0]),
                               sizeof(table[0]),
                               compare);

  if (ptr)
    return ptr->value;
  else
    {
      set_text(yylval, text);
      return IDENTIFIER;
    }
}

TOKEN string_literal(YYSTYPE *yylval, const char *text)
{
  set_text(yylval, text);
  return STRING_LITERAL;
}
