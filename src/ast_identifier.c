#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "symbol.h"
#include "util.h"

struct slots {
  const char *identifier;
  int line;
  SYMBOL *symbol;
};

size_t ast_identifier_size() { return SLOT_SIZE; }

static void add_symbols(NODE *node, void *symbols)
{
  S(node).symbol = symbol_table_add_global(symbols, S(node).identifier);
}

static void find_symbols(NODE *node, void *symbols)
{
  S(node).symbol = symbol_table_add_local(symbols, S(node).identifier);
}

static void generate_ir(NODE *node, IR *ir)
{
}

static void generate_param_ir(NODE *node, IR *ir)
{
  ir_add(ir, IR_PARAM, IR_SYM, S(node).symbol);
}

static SYMBOL *get_symbol(NODE *node)
{
  return S(node).symbol;
}

static IR_TYPE ir_type(NODE *node)
{
  return IR_SYM;
}

static void *ir_value(NODE *node)
{
  return S(node).symbol;
}

static int is_lvalue(NODE *node)
{
  return TRUE;
}

static void print(NODE *node, FILE *out)
{
  size_t length = strlen("AST_IDENTIFIER") + strlen(S(node).identifier) + 3;
  char *label = my_malloc(length * sizeof(char));
  snprintf(label, length, "AST_IDENTIFIER\\n%s", S(node).identifier);

  PRINT_NODE(out, node, label);

  free(label);
}

static const char *to_s(NODE *node)
{
  return S(node).identifier;
}

void ast_identifier_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, char *);
  S(node).line = va_arg(args, int);

  SET_METHODS(node);
  OVERRIDE(node, add_symbols);
  OVERRIDE(node, generate_param_ir);
  OVERRIDE(node, get_symbol);
  OVERRIDE(node, ir_type);
  OVERRIDE(node, ir_value);
  OVERRIDE(node, is_lvalue);
}
