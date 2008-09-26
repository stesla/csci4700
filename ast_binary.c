#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_binary_to_s(NODE *node)
{
  const char *left = ast_to_s(S(node).binary.left);
  const char *op = ast_op_str(S(node).binary.op);
  const char *right = ast_to_s(S(node).binary.right);
  size_t length = strlen(left) + strlen(op) + strlen(right) + 3;
  char *result = my_malloc(length *sizeof(char));
  snprintf(result, length, "%s %s %s", left, op, right);
  return result;
}

void ast_binary_init(NODE *node, va_list args)
{
  S(node).binary.op = va_arg(args, OP_TYPE);
  S(node).binary.left = va_arg(args, NODE *);
  S(node).binary.right = va_arg(args, NODE *);

  SET_M(node, ast_binary_to_s);
}
