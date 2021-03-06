#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"
#include "symbol.h"

struct slots {
  NODE *identifier;
  NODE *params;
  NODE *body;
  SYMBOL *symbol;
  SYMBOLS *psymbols;
  SYMBOLS *symbols;
  int end_line;
};

size_t ast_function_size() { return SLOT_SIZE; }

static void find_literals(NODE *node, LITERALS *literals)
{
  ast_find_literals(S(node).body, literals);
}

static void find_symbols(NODE *node, void *symbols)
{
  void *cur;
  const char *id = ast_to_s(S(node).identifier);

  S(node).symbol = symbol_table_add_function(symbols, id, S(node).params);
  S(node).psymbols = symbol_table_create(symbols, SYMBOL_DIRECTION_UP);
  ast_find_symbols(S(node).params, S(node).psymbols);
  S(node).symbols = symbol_table_create(S(node).psymbols, SYMBOL_DIRECTION_DOWN);
  ast_find_symbols(S(node).body, S(node).symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  size_t ar_size = symbol_table_size(S(node).symbols);
  int address = symbol_address(S(node).symbol);
  ir_reset_temp(ar_size);
  ir_add(ir, IR_LABEL, IR_CONST, &address);
  ir_add(ir, IR_ENTER);
  if (ar_size > 0)
    ir_add(ir, IR_PUSH, IR_CONST, &ar_size);
  ast_generate_ir(S(node).body, ir);
  ir_add(ir, IR_LEAVE);
  ir_add(ir, IR_RETURN, IR_NULL);
}

static void check_functions(NODE *node, SYMBOLS *symbols)
{
  ast_check_functions(S(node).body, symbols);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_FUNCTION");

  PRINT_EDGE(out, node, S(node).identifier);
  PRINT_EDGE(out, node, S(node).params);
  PRINT_EDGE(out, node, S(node).body);
}

static const char *to_s(NODE *node)
{
  char *result;
  const char *identifier = ast_to_s(S(node).identifier);
  const char *params = ast_to_s(S(node).params);
  size_t length = strlen(identifier) + strlen(params) + 3;
  result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s(%s)", identifier, params);
  return result;
}

void ast_function_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, NODE *);
  S(node).params = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);
  S(node).end_line = va_arg(args, int);

  SET_METHODS(node);
  OVERRIDE(node, find_literals);
  OVERRIDE(node, check_functions);
}
