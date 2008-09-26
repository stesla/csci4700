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

static const char *ast_for_to_s(NODE *node)
{
  return strdup("FOR");
}

void ast_for_init(NODE *node, va_list args)
{
  S(node).initializer = va_arg(args, NODE *);
  S(node).condition = va_arg(args, NODE *);
  S(node).increment = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);

  SET_M(node, ast_for_to_s);
}
