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

static int get_temp(NODE *node)
{
  return S(node).temp;
}

static void postop(NODE *node, IR *ir, IR_INST inst)
{
  SYMBOL *symbol;

  ir_add(ir, IR_ASSIGN,
         IR_TEMP, ast_get_temp(S(node).operand),
         IR_TEMP, S(node).temp);
  /* Only AST_IDENTIFIER or AST_FORMAL nodes will return a symbol here, but
   * it's legal for operand to be something else. The safe thing to do is to
   * just bail. */
  symbol = ast_get_symbol(S(node).operand);
  if (symbol)
    {
      ir_add(ir, inst,
             IR_TEMP, ast_get_temp(S(node).operand),
             IR_CONST, 1,
             IR_TEMP, ast_get_temp(S(node).operand));
      ir_add(ir, IR_ASSIGN,
             IR_TEMP, ast_get_temp(S(node).operand),
             IR_SYM, symbol);
    }
}

static void generate_ir(NODE *node, IR *ir)
{
  ast_generate_ir(S(node).operand, ir);
  switch(S(node).op)
    {
    case AST_OP_DEC:
      postop(node, ir, IR_SUBTRACT);
      break;
    case AST_OP_INC:
      postop(node, ir, IR_ADD);
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
  char label[16];
  snprintf(label, sizeof(label), "AST_POSTFIX\\n%s", ast_op_str(S(node).op));

  PRINT_NODE(out, node, label);

  PRINT_EDGE(out, node, S(node).operand);
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
  OVERRIDE(node, get_temp);
}
