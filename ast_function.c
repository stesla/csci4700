#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *identifier;
  NODE *params;
  NODE *body;
};

size_t ast_function_size() { return SLOT_SIZE; }

static void ast_function_print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_FUNCTION");

  PRINT_EDGE(out, node, S(node).identifier);
  PRINT_EDGE(out, node, S(node).params);
  PRINT_EDGE(out, node, S(node).body);
}

static const char *ast_function_to_s(NODE *node)
{
  char *result;
  const char *identifier = ast_to_s(S(node).identifier);
  const char *params = ast_to_s(S(node).params);
  size_t length = strlen(identifier) + strlen(params) + 3;
  result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s(%s)", identifier, params);
  return result;
}

void ast_function_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, NODE *);
  S(node).params = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);

  SET_M(node,
        ast_function_print,
        ast_function_to_s);
}
