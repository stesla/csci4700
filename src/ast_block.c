#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "symbol.h"
#include "util.h"

struct slots {
  NODE *declarations;
  NODE *statements;
  void *symbols;
  int start_line;
  int end_line;
};

size_t ast_block_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  void *cur;

  S(node).symbols = symbol_table_create(symbols);
  if (S(node).declarations)
    ast_find_symbols(S(node).declarations, S(node).symbols);
  if (S(node).statements)
    ast_find_symbols(S(node).statements, S(node).symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  size_t activation_record_size = symbol_table_size(S(node).symbols);
  ir_reset_temp();
  ir_add(ir, IR_PUSH, IR_CONST, activation_record_size);
  ast_generate_ir(S(node).statements, ir);
  ir_add(ir, IR_POP, IR_CONST, activation_record_size);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_BLOCK");
  PRINT_EDGE(out, node, S(node).declarations);
  PRINT_EDGE(out, node, S(node).statements);
}

static const char *to_s(NODE *node)
{
  const char *result;

  if (S(node).declarations && S(node).statements) { result = "BLOCK {D S}"; }
  else if (!S(node).declarations && !S(node).statements ) { result ="BLOCK {}"; }
  else if (!S(node).declarations) { result = "BLOCK {S}"; }
  else { result = "BLOCK {D}"; }

  return strdup(result);
}

void ast_block_init(NODE *node, va_list args)
{
  S(node).declarations = va_arg(args, NODE *);
  S(node).statements = va_arg(args, NODE *);
  S(node).start_line = va_arg(args, int);
  S(node).end_line = va_arg(args, int);

  SET_METHODS(node);
}
