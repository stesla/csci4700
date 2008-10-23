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

static void preop(NODE *node, IR *ir, IR_INST inst)
{
  SYMBOL *symbol;
  int one = 1;

  ir_add(ir, inst,
         ast_ir_type(S(node).operand), ast_ir_value(S(node).operand),
         IR_CONST, &one,
         ast_ir_type(node), ast_ir_value(node));
  /* Only AST_IDENTIFIER or AST_FORMAL nodes will return a symbol here, but
   * it's legal for operand to be something else. The safe thing to do is
   * return the altered value, but throw it away. */
  symbol = ast_get_symbol(S(node).operand);
  if (symbol)
    ir_add(ir, IR_ASSIGN, ast_ir_type(node), ast_ir_value(node), IR_SYM, symbol);
}

static void generate_ir(NODE *node, IR *ir)
{
  int label_a, label_b;
  int one = 1;
  int zero = 0;

  S(node).temp = ir_make_temp(ir);

  /* TODO: Need to implement REF and DEREF */
  ast_generate_ir(S(node).operand, ir);
  switch(S(node).op)
    {
    case AST_OP_DEC:
      preop(node, ir, IR_SUBTRACT);
      break;
    case AST_OP_DEREF:
      /* TODO: Leaving this out on purpose */
      break;
    case AST_OP_INC:
      preop(node, ir, IR_ADD);
      break;
    case AST_OP_MINUS:
      ir_add(ir, IR_SUBTRACT,
             IR_CONST, &zero,
             ast_ir_type(S(node).operand), ast_ir_value(S(node).operand),
             ast_ir_type(node), ast_ir_value(node));
      break;
    case AST_OP_NOT:
      /* IF operand == 0 THEN JUMP A
         result = 0
         JUMP B
         A: result = 1
         B: */
      label_a = ir_make_label();
      label_b = ir_make_label();
      ir_add(ir, IR_IF_EQ,
             ast_ir_type(S(node).operand), ast_ir_value(S(node).operand),
             IR_CONST, &zero,
             IR_CONST, &label_a);
      ir_add(ir, IR_ASSIGN,
             IR_CONST, &zero,
             ast_ir_type(node), ast_ir_value(node));
      ir_add(ir, IR_JUMP, IR_CONST, &label_b);
      ir_add(ir, IR_LABEL, IR_CONST, &label_a);
      ir_add(ir, IR_ASSIGN,
             IR_CONST, &one,
             ast_ir_type(node), ast_ir_value(node));
      ir_add(ir, IR_LABEL, IR_CONST, &label_b);
      break;
    case AST_OP_REF:
      /* TODO: Leaving this out on purpose */
      break;
    default:
      ;
    }
}

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).operand, symbols);
}

static IR_TYPE ir_type(NODE *node)
{
  return IR_TEMP;
}

static void *ir_value(NODE *node)
{
  void *result = &(S(node).temp);
  return result;
}

static void print(NODE *node, FILE *out)
{
  char label[15];
  snprintf(label, sizeof(label), "AST_PREFIX\\n%s", ast_op_str(S(node).op));

  PRINT_NODE(out, node, label);

  PRINT_EDGE(out, node, S(node).operand);
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
  OVERRIDE(node, ir_type);
  OVERRIDE(node, ir_value);
}
