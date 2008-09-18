#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include "parser.h"

extern int yyparse(void);

/* lexer debugging flag */
extern int yyget_debug(void);
extern void yyset_debug(int value);

/* parser debugging flag */
extern int yydebug;

int usage( void )
{
  printf("usage: whatever <file> [-l] [-y]\n");
  printf("-l == turns on lex debug flag\n");
  printf("-s == enable a listing of program being compiled\n");
  printf("-y == turns on yydebug flag\n");
  return( 1 );
}

int main(int argc, char **argv)
{
  char *filename;
  int  i;
  int result;

  /* Default debugging to off */
  yyset_debug(FALSE);

  if ( argc < 2 )
    exit( usage() );
  else
    for( i = 1; i < argc; i++ )
      if ( argv[ i ][ 0 ] == '-' )
        switch( argv[ i ][ 1 ] )
          {
          case 'l':
            yyset_debug(!yyget_debug());
            break;
          case 's':
            toggle_listing();
            break;
          case 'y':
            yydebug = ! yydebug;
            break;
          default:
            exit(usage());
          }
      else
        filename = argv[ i ];

  if (open_input(filename) < 0)
    {
      perror("open_input");
      exit(1);
    }

  result = yyparse();

  if (close_input() < 0)
    {
      perror("close_input");
      exit(1);
    }

  if (result)
    {
      fprintf(stderr, "Failed to compile\n");
      return(1);
    }
  else
    {
      printf("Compiled OK!\n");
      return(0);
    }
}
