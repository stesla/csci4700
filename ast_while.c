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

static const char *ast_while_to_s(NODE *node)
{
  return strdup("WHILE");
}

void ast_while_init(NODE *node, va_list args)
{
  S(node).condition = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);

  SET_M(node, ast_while_to_s);
}
