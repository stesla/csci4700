#include <stdlib.h>
#include <stdio.h>
#include "input.h"
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
  char *filename;
  int  i, success;
  NODE *ast;

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

  if (open_input(filename) < 0)
    {
      perror("open_input");
      exit(1);
    }

  success = semantic_analysis(&ast, lexer_debug, parser_debug);

  if (close_input() < 0)
    {
      perror("close_input");
      exit(1);
    }

  return success;
}
