#include <stdarg.h>
#include <stdio.h>
#include "ast.h"

struct slots {
  OP_TYPE op;
  NODE *left;
  NODE *right;
};

size_t ast_binary_size() { return SLOT_SIZE; }

static void ast_binary_print(NODE *node, FILE *out)
{
  char label[15]; /* AST_BINARY ++ */
  snprintf(label, 14, "AST_BINARY\\n%s", ast_op_str(S(node).op));

  PRINT_NODE(out, node, label);
  PRINT_EDGE(out, node, S(node).left);
  PRINT_EDGE(out, node, S(node).right);
}

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
  S(node).op = va_arg(args, OP_TYPE);
  S(node).left = va_arg(args, NODE *);
  S(node).right = va_arg(args, NODE *);

  SET_M(node,
        ast_binary_print,
        ast_binary_to_s);
}
