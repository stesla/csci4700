#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *condition;
  NODE *body;
};

size_t ast_while_size() { return SLOT_SIZE; }

static void find_literals(NODE *node, LITERALS *literals)
{
  ast_find_literals(S(node).body, literals);
}

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).condition, symbols);
  ast_find_symbols(S(node).body, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  int label_a = ir_make_label();
  int label_b = ir_make_label();
  ir_add(ir, IR_LABEL, IR_CONST, &label_a);
  ast_generate_ir(S(node).condition, ir);
  ir_add(ir, IR_IF_FALSE,
         ast_ir_type(S(node).condition), ast_ir_value(S(node).condition),
         IR_CONST, &label_b);
  ast_generate_ir(S(node).body, ir);
  ir_add(ir, IR_JUMP, IR_CONST, &label_a);
  ir_add(ir, IR_LABEL, IR_CONST, &label_b);
}

static void check_functions(NODE *node, SYMBOLS *symbols)
{
  ast_check_functions(S(node).body, symbols);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_WHILE");

  PRINT_EDGE(out, node, S(node).condition);
  PRINT_EDGE(out, node, S(node).body);
}

static const char *to_s(NODE *node)
{
  return strdup("WHILE");
}

void ast_while_init(NODE *node, va_list args)
{
  S(node).condition = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);

  SET_METHODS(node);
  OVERRIDE(node, find_literals);
  OVERRIDE(node, check_functions);
}
