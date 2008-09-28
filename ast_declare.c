#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *list;
  int line;
};

size_t ast_declare_size() { return SLOT_SIZE; }

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_DECLARE");
  PRINT_EDGE(out, node, S(node).list);
}

static const char *to_s(NODE *node)
{
  return ast_to_s(S(node).list);
}

void ast_declare_init(NODE *node, va_list args)
{
  S(node).list = va_arg(args, NODE *);
  S(node).line = va_arg(args, int);

  SET_METHODS(node);
}
