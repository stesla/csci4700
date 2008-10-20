#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *operand;
  OP_TYPE op;
  int temp;
};

size_t ast_postfix_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).operand, symbols);
}

static void print(NODE *node, FILE *out)
{
  char label[16];
  snprintf(label, sizeof(label), "AST_POSTFIX\\n%s", ast_op_str(S(node).op));

  PRINT_NODE(out, node, label);

  PRINT_EDGE(out, node, S(node).operand);
}

static void set_temps(NODE *node, int val)
{
  S(node).temp = val;
  ast_set_temps(S(node).operand, val);
}

static const char *to_s(NODE *node)
{
  const char *operand = ast_to_s(S(node).operand);
  const char *op = ast_op_str(S(node).op);
  size_t length = strlen(operand) + strlen(op) + 1;
  char *result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s%s", operand, op);
  return result;
}

void ast_postfix_init(NODE *node, va_list args)
{
  S(node).operand = va_arg(args, NODE *);
  S(node).op = va_arg(args, OP_TYPE);

  SET_METHODS(node);
}
