#include <stdarg.h>
#include <stdio.h>
#include "ast.h"

struct slots {
  OP_TYPE op;
  NODE *left;
  NODE *right;
};

static const char *ast_binary_to_s(NODE *node)
{
  const char *left = ast_to_s(S(node).left);
  const char *op = ast_op_str(S(node).op);
  const char *right = ast_to_s(S(node).right);
  size_t length = strlen(left) + strlen(op) + strlen(right) + 3;
  char *result = my_malloc(length *sizeof(char));
  snprintf(result, length, "%s %s %s", left, op, right);
  return result;
}

void ast_binary_init(NODE *node, va_list args)
{
  ALLOC_S(node);

  S(node).op = va_arg(args, OP_TYPE);
  S(node).left = va_arg(args, NODE *);
  S(node).right = va_arg(args, NODE *);

  SET_M(node, ast_binary_to_s);
}
