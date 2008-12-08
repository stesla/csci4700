#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

/* TODO: Do we really need this node? It's not like we're pretty-printing
 * expressions a lot, and it just adds a level of hierarchy.
 */

struct slots {
  NODE *inner;
};

size_t ast_group_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).inner, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  ast_generate_ir(S(node).inner, ir);
}

static IR_TYPE ir_type(NODE *node)
{
  return ast_ir_type(S(node).inner);
}

static void *ir_value(NODE *node)
{
  return ast_ir_value(S(node).inner);
}

static void hook_functions(NODE *node, SYMBOLS *symbols)
{
  ast_hook_functions(S(node).inner, symbols);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_GROUP");
  PRINT_EDGE(out, node, S(node).inner);
}

static const char *to_s(NODE *node)
{
  const char *group = ast_to_s(S(node).inner);
  size_t length = strlen(group) + 3;
  char *result = my_malloc(length * sizeof(char));
  snprintf(result, length, "(%s)", group);
  return result;
}

void ast_group_init(NODE *node, va_list args)
{
  S(node).inner = va_arg(args, NODE *);

  SET_METHODS(node);
  OVERRIDE(node, hook_functions);
  OVERRIDE(node, ir_type);
  OVERRIDE(node, ir_value);
}
