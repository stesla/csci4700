#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *inner;
};

size_t ast_group_size() { return SLOT_SIZE; }

static void ast_group_print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_GROUP");
  PRINT_EDGE(out, node, S(node).inner);
}

static const char *ast_group_to_s(NODE *node)
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

  SET_M(node,
        ast_group_print,
        ast_group_to_s);
}
