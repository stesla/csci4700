#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_prefix_to_s(NODE *node)
{
  const char *operand = ast_to_s(S(node).unary.operand);
  const char *op = ast_op_str(S(node).unary.op);
  size_t length = strlen(operand) + strlen(op) + 1;
  char *result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s%s", op, operand);
  return result;
}

void ast_prefix_init(NODE *node, va_list args)
{
  S(node).unary.operand = va_arg(args, NODE *);
  S(node).unary.op = va_arg(args, OP_TYPE);

  SET_M(node, ast_prefix_to_s);
}
