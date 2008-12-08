#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "ir.h"
#include "parser.h"
#include "pki.h"
#include "symbol.h"

int usage( void )
{
  printf("usage: whatever <file> [options]\n");
  printf("-a -- Produce a GraphViz visualization of the AST.\n");
  printf("-i -- Output the intermediate representation.\n");
  printf("-l -- Enable lexer debugging.\n");
  printf("-y -- Enable parser debugging.\n");
  return( 1 );
}

static void add_main_invocation(IR *ir, SYMBOLS *symbols);

int main(int argc, char **argv)
{
  const char *filename;
  FILE *out_file;
  char out_file_name[FILENAME_MAX];
  int  i, success;
  NODE *ast = NULL;
  SYMBOLS *symbols;
  LITERALS *literals;
  IR *ir;

  /* CLI flags */
  int lexer_debug = FALSE;
  int output_ast = FALSE;
  int output_ir = FALSE;
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
          case 'i':
            output_ir = !output_ir;
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
      FILE *out_file;
      ext(filename, ".dot", out_file_name, sizeof(out_file_name));
      out_file = fopen(out_file_name, "w");
      ast_print(ast, out_file);
      fclose(out_file);
    }

  /* Fill Tables */
  symbols = symbol_table_create(NULL, SYMBOL_DIRECTION_DOWN);
  ast_find_symbols(ast, symbols);
  literals = literal_table_create();
  ast_find_literals(ast, literals);
  ast_check_functions(ast, symbols);

  /* Generate IR */
  ir = ir_create();
  add_main_invocation(ir, symbols);
  ast_generate_ir(ast, ir);
  if (output_ir)
    {
      FILE *out_file;
      ext(filename, ".l", out_file_name, sizeof(out_file_name));
      out_file = fopen(out_file_name, "w");
      ir_fprint(out_file, ir);
      fclose(out_file);
    }

  /* Generate PKI */
  ext(filename, ".s", out_file_name, sizeof(out_file_name));
  out_file = fopen(out_file_name, "w");
  pki_generate(out_file, ir);
  pki_generate_globals(out_file, symbols);
  pki_generate_literals(out_file, literals);
  fclose(out_file);

  return 0;
}

static void add_main_invocation(IR *ir, SYMBOLS *symbols)
{
  SYMBOL *main = symbol_table_find(symbols, "main");
  if (main == NULL || ! symbol_is_function(main))
    {
      fprintf(stderr, "Could not find main function.\n");
      exit(1);
    }
  ir_add(ir, IR_CALL, IR_SYM, main);
  ir_add(ir, IR_HALT);
}
