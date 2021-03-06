#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *identifier;
};

size_t ast_read_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).identifier, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  ir_add(ir, IR_READ,
         ast_ir_type(S(node).identifier), ast_ir_value(S(node).identifier));
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_READ");

  PRINT_EDGE(out, node, S(node).identifier);
}

static const char *to_s(NODE *node)
{
  char *result;
  const char *identifier = ast_to_s(S(node).identifier);
  size_t length = strlen(identifier) + strlen("READ()") + 1;
  result = my_malloc(length * sizeof(char));
  snprintf(result, length, "READ(%s)", identifier);
  return result;
}

void ast_read_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, NODE *);

  SET_METHODS(node);
}
