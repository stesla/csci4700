#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *condition;
  NODE *body;
};

size_t ast_while_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).condition, symbols);
  ast_find_symbols(S(node).body, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  /* TODO:IR */
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_WHILE");

  PRINT_EDGE(out, node, S(node).condition);
  PRINT_EDGE(out, node, S(node).body);
}

static void set_temps(NODE *node, int val)
{
  ast_set_temps(S(node).condition, val);
  ast_set_temps(S(node).body, val);
}

static const char *to_s(NODE *node)
{
  return strdup("WHILE");
}

void ast_while_init(NODE *node, va_list args)
{
  S(node).condition = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);

  SET_METHODS(node);
}
