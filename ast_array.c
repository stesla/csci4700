#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *identifier;
  NODE *count;
};

size_t ast_array_size() { return SLOT_SIZE; }

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_ARRAY");
  PRINT_EDGE(out, node, S(node).identifier);
  PRINT_EDGE(out, node, S(node).count);
}

static const char *to_s(NODE *node)
{
  const char *identifier = ast_to_s(S(node).identifier);
  const char *count = ast_to_s(S(node).count);
  size_t length = strlen(identifier) + strlen(count) + 3; /* first[count] */
  char *result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s[%s]", identifier, count);
  return result;
}

void ast_array_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, NODE *);
  S(node).count = va_arg(args, NODE *);

  SET_METHODS(node);
}
