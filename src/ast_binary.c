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

static void check_functions(NODE *node, SYMBOLS *symbols)
{
  ast_check_functions(S(node).right, symbols);
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
         ast_ir_type(S(node).left), ast_ir_value(S(node).left),
         ast_ir_type(S(node).right), ast_ir_value(S(node).right),
         ast_ir_type(node), ast_ir_value(node));
}

static void ir_rel_op(NODE *node, IR *ir, IR_INST inst)
{
  int label_a, label_b;
  int one = 1;
  int zero = 0;
  ast_generate_ir(S(node).left, ir);
  ast_generate_ir(S(node).right, ir);
  label_a = ir_make_label();
  label_b = ir_make_label();
  ir_add(ir, inst,
         ast_ir_type(S(node).left), ast_ir_value(S(node).left),
         ast_ir_type(S(node).right), ast_ir_value(S(node).right),
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
}

static void ir_short_op(NODE *node, IR *ir, IR_INST inst, int v1, int v2)
{
  int label_a = ir_make_label();
  int label_b = ir_make_label();
  ast_generate_ir(S(node).left, ir);
  ir_add(ir, inst,
         ast_ir_type(S(node).left), ast_ir_value(S(node).left),
         IR_CONST, &label_a);
  ast_generate_ir(S(node).right, ir);
  ir_add(ir, inst,
         ast_ir_type(S(node).right), ast_ir_value(S(node).right),
         IR_CONST, &label_a);
  ir_add(ir, IR_ASSIGN, IR_CONST, &v1, ast_ir_type(node), ast_ir_value(node));
  ir_add(ir, IR_JUMP, IR_CONST, &label_b);
  ir_add(ir, IR_LABEL, IR_CONST, &label_a);
  ir_add(ir, IR_ASSIGN, IR_CONST, &v2, ast_ir_type(node), ast_ir_value(node));
  ir_add(ir, IR_LABEL, IR_CONST, &label_b);
}

static int is_indirect(NODE *node)
{
  return ast_get_symbol(node) == NULL;
}

static void ir_assign(NODE *node, IR *ir)
{
  /* rvalue */
  ast_generate_ir(S(node).right, ir);

  /* temporary var for this expression */
  ir_add(ir, IR_ASSIGN,
         ast_ir_type(S(node).right), ast_ir_value(S(node).right),
         ast_ir_type(node), ast_ir_value(node));

  /* lvalue */
  ast_generate_lval_ir(S(node).left, ir);
  ir_add(ir, is_indirect(S(node).left) ? IR_ASSIGN_INDIRECT : IR_ASSIGN,
         ast_ir_type(node), ast_ir_value(node),
         ast_ir_type(S(node).left), ast_ir_value(S(node).left));
}

static void generate_ir(NODE *node, IR *ir)
{
  S(node).temp = ir_make_temp(ir);
  switch (S(node).op)
    {
    case AST_OP_ASSIGN:
      ir_assign(node, ir);
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
      ir_short_op(node, ir, IR_IF_FALSE, 1, 0);
      break;
    case AST_OP_LE:
      ir_rel_op(node, ir, IR_IF_LE);
      break;
    case AST_OP_LOR:
      ir_short_op(node, ir, IR_IF_TRUE, 0, 1);
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

static IR_TYPE ir_type(NODE *node)
{
  return IR_TEMP;
}

static void *ir_value(NODE *node)
{
  return &(S(node).temp);
}

static void print(NODE *node, FILE *out)
{
  char label[15]; /* AST_BINARY ++ */
  snprintf(label, 14, "AST_BINARY\\n%s", ast_op_str(S(node).op));

  PRINT_NODE(out, node, label);
  PRINT_EDGE(out, node, S(node).left);
  PRINT_EDGE(out, node, S(node).right);
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
  OVERRIDE(node, check_functions);
  OVERRIDE(node, ir_type);
  OVERRIDE(node, ir_value);
}
