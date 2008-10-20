#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *initializer;
  NODE *condition;
  NODE *increment;
  NODE *body;
};

size_t ast_for_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).initializer, symbols);
  ast_find_symbols(S(node).condition, symbols);
  if (S(node).increment)
    ast_find_symbols(S(node).increment, symbols);
  ast_find_symbols(S(node).body, symbols);
}

void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_FOR");

  PRINT_EDGE(out, node, S(node).initializer);
  PRINT_EDGE(out, node, S(node).condition);
  if (S(node).increment)
    PRINT_EDGE(out, node, S(node).increment);
  PRINT_EDGE(out, node, S(node).body);
}

static void set_temps(NODE *node, int val)
{
  ast_set_temps(S(node).initializer, val);
  ast_set_temps(S(node).condition, val);
  ast_set_temps(S(node).increment, val);
  ast_set_temps(S(node).body, val);
}

static const char *to_s(NODE *node)
{
  return strdup("FOR");
}

void ast_for_init(NODE *node, va_list args)
{
  S(node).initializer = va_arg(args, NODE *);
  S(node).condition = va_arg(args, NODE *);
  S(node).increment = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);

  SET_METHODS(node);
}
