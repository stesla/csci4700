#include <stdarg.h>
#include <stdio.h>
#include "ast.h"

struct slots {
  OP_TYPE op;
  NODE *left;
  NODE *right;
  int temp;
};

size_t ast_binary_size() { return SLOT_SIZE; }

static int get_temp(NODE *node)
{
  return S(node).temp;
}

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).left, symbols);
  ast_find_symbols(S(node).right, symbols);
}

static void ir_binary_op(NODE *node, IR *ir, IR_INST inst)
{
  ast_generate_ir(S(node).left, ir);
  ast_generate_ir(S(node).right, ir);
  ir_add(ir, inst,
         IR_TEMP, ast_get_temp(S(node).left),
         IR_TEMP, ast_get_temp(S(node).right),
         IR_TEMP, S(node).temp);
}

static void ir_rel_op(NODE *node, IR *ir, IR_INST inst)
{
  /* IF arg1 OP arg2 THEN JUMP A
     result = 0
     JUMP B
     A: result = 1
     B: */
  int label_a, label_b;
  ast_generate_ir(S(node).left, ir);
  ast_generate_ir(S(node).right, ir);
  label_a = ir_make_label();
  label_b = ir_make_label();
  ir_add(ir, inst,
         IR_TEMP, ast_get_temp(S(node).left),
         IR_TEMP, ast_get_temp(S(node).right),
         IR_CONST, label_a);
  ir_add(ir, IR_ASSIGN,
         IR_CONST, 0,
         IR_TEMP, S(node).temp);
  ir_add(ir, IR_JUMP, IR_CONST, label_b);
  ir_add(ir, IR_LABEL, IR_CONST, label_a);
  ir_add(ir, IR_ASSIGN,
         IR_CONST, 1,
         IR_TEMP, S(node).temp);
  ir_add(ir, IR_LABEL, IR_CONST, label_b);
}

static void generate_ir(NODE *node, IR *ir)
{
  /* TODO: Need to implement logic operators, short circuits, and assignment */
  switch (S(node).op)
    {
    case AST_OP_ASSIGN:
      ast_generate_ir(S(node).right, ir);
      /* In order to make a = b = c work, we need to put our value into both a
       * temp register and into the actual variable. */
      ir_add(ir, IR_ASSIGN,
             IR_TEMP, ast_get_temp(S(node).right),
             IR_TEMP, S(node).temp);
      ir_add(ir, IR_ASSIGN,
             IR_TEMP, ast_get_temp(S(node).right),
             IR_SYM, ast_get_symbol(S(node).left));
      break;
    case AST_OP_BAND:
      ir_binary_op(node, ir, IR_AND);
      break;
    case AST_OP_BOR:
      ir_binary_op(node, ir, IR_OR);
      break;
    case AST_OP_BXOR:
      ir_binary_op(node, ir, IR_XOR);
      break;
    case AST_OP_DIV:
      ir_binary_op(node, ir, IR_DIVIDE);
      break;
    case AST_OP_EQ:
      ir_rel_op(node, ir, IR_IF_EQ);
      break;
    case AST_OP_GE:
      ir_rel_op(node, ir, IR_IF_GE);
      break;
    case AST_OP_GT:
      ir_rel_op(node, ir, IR_IF_GT);
      break;
    case AST_OP_LAND:
      /* TODO: Short Circuit */
      break;
    case AST_OP_LE:
      ir_rel_op(node, ir, IR_IF_LE);
      break;
    case AST_OP_LOR:
      /* TODO: Short Circuit */
      break;
    case AST_OP_LT:
      ir_rel_op(node, ir, IR_IF_LT);
      break;
    case AST_OP_MINUS:
      ir_binary_op(node, ir, IR_SUBTRACT);
      break;
    case AST_OP_MOD:
      ir_binary_op(node, ir, IR_MODULO);
      break;
    case AST_OP_MULT:
      ir_binary_op(node, ir, IR_MULTIPLY);
      break;
    case AST_OP_NE:
      ir_rel_op(node, ir, IR_IF_NE);
      break;
    case AST_OP_PLUS:
      ir_binary_op(node, ir, IR_ADD);
      break;
    default:
      ;
    }
}

static void print(NODE *node, FILE *out)
{
  char label[15]; /* AST_BINARY ++ */
  snprintf(label, 14, "AST_BINARY\\n%s", ast_op_str(S(node).op));

  PRINT_NODE(out, node, label);
  PRINT_EDGE(out, node, S(node).left);
  PRINT_EDGE(out, node, S(node).right);
}

static void set_temps(NODE *node, int val)
{
  S(node).temp = val;
  ast_set_temps(S(node).left, val);
  ast_set_temps(S(node).right, val + 1);
}

static const char *to_s(NODE *node)
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

  SET_METHODS(node);
  OVERRIDE(node, get_temp);
}
