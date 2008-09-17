#include <stdio.h>
#include "y.tab.h"

struct tab {
  char word[ 9 ];
  int value;
};

static int comp( const void *a, const void *b )
{
  return( strcmp( ( ( struct tab *) a ) -> word,
                  ( ( struct tab *) b ) -> word ) );
}


int is_keyword( const char *string )
{
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

  ptr = (struct tab *) bsearch( string, (char *) &table[ 0 ], sizeof( table ) /
                                sizeof( table[ 0 ] ), sizeof( table[ 0 ] ), comp );
  if ( ptr )
    return( ptr -> value );
  else
    return( IDENTIFIER );
}

void consume_comment(void)
{
  /* These are defined in lexer.c */
  extern int yy_flex_debug;
  extern int next_char(void);

  while (next_char() != '\n')
    ;
  if (yy_flex_debug)
    printf("Removed comment from input stream.\n");
}
