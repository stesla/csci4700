#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *value;
};

size_t ast_write_size() { return SLOT_SIZE; }

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_WRITE");

  PRINT_EDGE(out, node, S(node).value);
}

static const char *to_s(NODE *node)
{
  char *result;
  const char *value = ast_to_s(S(node).value);
  size_t length = strlen(value) + strlen("WRITE()") + 1;
  result = my_malloc(length * sizeof(char));
  snprintf(result, length, "WRITE(%s)", value);
  return result;
}

void ast_write_init(NODE *node, va_list args)
{
  S(node).value = va_arg(args, NODE *);

  SET_M(node,
        print,
        to_s);
}
