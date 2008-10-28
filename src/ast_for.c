#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *initializer;
  NODE *condition;
  NODE *increment;
  NODE *body;
};

size_t ast_for_size() { return SLOT_SIZE; }

static void find_literals(NODE *node, LITERALS *literals)
{
  if (S(node).body)
    ast_find_literals(S(node).body, literals);
}

static void find_symbols(NODE *node, void *symbols)
{
  if (S(node).initializer)
    ast_find_symbols(S(node).initializer, symbols);
  if (S(node).condition)
    ast_find_symbols(S(node).condition, symbols);
  if (S(node).increment)
    ast_find_symbols(S(node).increment, symbols);
  if (S(node).body)
    ast_find_symbols(S(node).body, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  int label_a = ir_make_label();
  int label_b = ir_make_label();
  if (S(node).initializer)
    ast_generate_ir(S(node).initializer, ir);
  ir_add(ir, IR_LABEL, IR_CONST, &label_a);
  if (S(node).condition)
    {
      ast_generate_ir(S(node).condition, ir);
      ir_add(ir, IR_IF_FALSE,
             ast_ir_type(S(node).condition), ast_ir_value(S(node).condition),
             IR_CONST, &label_b);
    }
  if (S(node).body)
    ast_generate_ir(S(node).body, ir);
  if (S(node).increment)
    ast_generate_ir(S(node).increment, ir);
  ir_add(ir, IR_JUMP, IR_CONST, &label_a);
  ir_add(ir, IR_LABEL, IR_CONST, &label_b);
}

void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_FOR");

  PRINT_EDGE(out, node, S(node).initializer);
  PRINT_EDGE(out, node, S(node).condition);
  if (S(node).increment)
    PRINT_EDGE(out, node, S(node).increment);
  PRINT_EDGE(out, node, S(node).body);
}

static const char *to_s(NODE *node)
{
  return strdup("FOR");
}

void ast_for_init(NODE *node, va_list args)
{
  S(node).initializer = va_arg(args, NODE *);
  S(node).condition = va_arg(args, NODE *);
  S(node).increment = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);

  SET_METHODS(node);
  OVERRIDE(node, find_literals);
}
