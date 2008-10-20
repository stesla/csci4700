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

size_t ast_prefix_size() { return SLOT_SIZE; }

static int get_temp(NODE *node)
{
  return S(node).temp;
}

static void generate_ir(NODE *node, IR *ir)
{
  /* TODO: need get_symbol method for these */
  ast_generate_ir(S(node).operand, ir);
  switch(S(node).op)
    {
    case AST_OP_DEC:
      break;
    case AST_OP_DEREF:
      break;
    case AST_OP_INC:
      break;
    case AST_OP_MINUS:
      ir_add(ir, IR_SUBTRACT,
             IR_CONST, 0,
             IR_TEMP, ast_get_temp(S(node).operand),
             IR_TEMP, S(node).temp);
      break;
    case AST_OP_REF:
      break;
    default:
      ;
    }
}

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).operand, symbols);
}

static void print(NODE *node, FILE *out)
{
  char label[15];
  snprintf(label, sizeof(label), "AST_PREFIX\\n%s", ast_op_str(S(node).op));

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
  snprintf(result, length, "%s%s", op, operand);
  return result;
}

void ast_prefix_init(NODE *node, va_list args)
{
  S(node).operand = va_arg(args, NODE *);
  S(node).op = va_arg(args, OP_TYPE);

  SET_METHODS(node);
  OVERRIDE(node, get_temp);
}
