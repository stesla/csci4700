#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *condition;
  NODE *if_branch;
  NODE *else_branch;
};

size_t ast_conditional_size() { return SLOT_SIZE; }

static void find_literals(NODE *node, LITERALS *literals)
{
  if (S(node).if_branch)
    ast_find_literals(S(node).if_branch, literals);
  if (S(node).else_branch)
    ast_find_literals(S(node).else_branch, literals);
}

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).condition, symbols);
  ast_find_symbols(S(node).if_branch, symbols);
  if (S(node).else_branch)
    ast_find_symbols(S(node).else_branch, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  int label_a = ir_make_label();
  ast_generate_ir(S(node).condition, ir);
  ir_add(ir, IR_IF_FALSE,
         ast_ir_type(S(node).condition), ast_ir_value(S(node).condition),
         IR_CONST, &label_a);
  ast_generate_ir(S(node).if_branch, ir);
  if (S(node).else_branch)
    {
      int label_b = ir_make_label();
      ir_add(ir, IR_JUMP, IR_CONST, &label_b);
      ir_add(ir, IR_LABEL, IR_CONST, &label_a);
      ast_generate_ir(S(node).else_branch, ir);
      ir_add(ir, IR_LABEL, IR_CONST, &label_b);
    }
  else
    ir_add(ir, IR_LABEL, IR_CONST, &label_a);
}

static void check_functions(NODE *node, SYMBOLS *symbols)
{
  if (S(node).if_branch)
    ast_check_functions(S(node).if_branch, symbols);
  if (S(node).else_branch)
    ast_check_functions(S(node).else_branch, symbols);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_CONDITIONAL");

  PRINT_EDGE(out, node, S(node).condition);
  PRINT_EDGE(out, node, S(node).if_branch);
  if (S(node).else_branch)
    PRINT_EDGE(out, node, S(node).else_branch);
}

static const char *to_s(NODE *node)
{
  char *result;
  size_t length;
  const char *condition = ast_to_s(S(node).condition);

  if(S(node).else_branch == NULL)
    {
      length = strlen(condition) + strlen("IF()") + 1;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "IF(%s)", condition);
    }
  else
    {
      length = strlen(condition) + strlen("IF()ELSE") + 1;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "IF(%s)", condition);
    }

  return result;
}

void ast_conditional_init(NODE *node, va_list args)
{
  S(node).condition = va_arg(args, NODE *);
  S(node).if_branch = va_arg(args, NODE *);
  S(node).else_branch = va_arg(args, NODE *);

  SET_METHODS(node);
  OVERRIDE(node, find_literals);
  OVERRIDE(node, check_functions);
}
