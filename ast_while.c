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

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_WHILE");

  PRINT_EDGE(out, node, S(node).condition);
  PRINT_EDGE(out, node, S(node).body);
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
