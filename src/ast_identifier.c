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
  int temp;
};

size_t ast_identifier_size() { return SLOT_SIZE; }

static int get_temp(NODE *node)
{
  return S(node).temp;
}

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
  ir_add(ir, IR_ASSIGN, IR_SYM, S(node).symbol, IR_TEMP, S(node).temp);
}

static void print(NODE *node, FILE *out)
{
  size_t length = strlen("AST_IDENTIFIER") + strlen(S(node).identifier) + 3;
  char *label = my_malloc(length * sizeof(char));
  snprintf(label, length, "AST_IDENTIFIER\\n%s", S(node).identifier);

  PRINT_NODE(out, node, label);

  free(label);
}

static void set_temps(NODE *node, int val)
{
  S(node).temp = val;
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
  OVERRIDE(node, get_temp);
}
