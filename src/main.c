#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "ir.h"
#include "parser.h"
#include "symbol.h"

int usage( void )
{
  printf("usage: whatever <file> [-l] [-y]\n");
  printf("-a -- Produce a GraphViz representation of the AST in the file ast.dot.\n");
  printf("-l -- Enable lexer debugging.\n");
  printf("-y -- Enable parser debugging.\n");
  return( 1 );
}

int main(int argc, char **argv)
{
  const char *filename;
  int  i, success;
  NODE *ast = NULL;
  void *symbols;
  void *ir;

  /* CLI flags */
  int lexer_debug = FALSE;
  int output_ast = FALSE;
  int parser_debug = FALSE;

  if ( argc < 2 )
    exit( usage() );
  else
    for( i = 1; i < argc; i++ )
      if ( argv[ i ][ 0 ] == '-' )
        switch( argv[ i ][ 1 ] )
          {
          case 'a':
            output_ast = !output_ast;
            break;
          case 'l':
            lexer_debug = !lexer_debug;
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

  /* Output AST */
  if (output_ast)
    {
      FILE *ast_file = fopen("ast.dot", "w");
      ast_print(ast, ast_file);
      fclose(ast_file);
    }

  /* Fill Symbol Table */
  symbols = symbol_table_create(NULL);
  ast_find_symbols(ast, symbols);

  /* Produce IR */
  ir = ir_create();
  ast_generate_ir(ast, ir);
  ir_fprint(stdout, ir);

  return 0;
}
