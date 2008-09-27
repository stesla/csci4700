#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

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
  const char *filename;
  int  i, success;
  NODE *ast = NULL;

  /* CLI flags */
  int lexer_debug = FALSE;
  int parser_debug = FALSE;

  if ( argc < 2 )
    exit( usage() );
  else
    for( i = 1; i < argc; i++ )
      if ( argv[ i ][ 0 ] == '-' )
        switch( argv[ i ][ 1 ] )
          {
          case 'l':
            lexer_debug = !lexer_debug;
            break;
          case 's':
            toggle_listing();
            break;
          case 'y':
            parser_debug = !parser_debug;
            break;
          default:
            exit(usage());
          }
      else
        filename = argv[ i ];

  /* Parse */
  if ((ast = semantic_analysis(filename, lexer_debug, parser_debug)) == NULL)
    return 1;

  return 0;
}
