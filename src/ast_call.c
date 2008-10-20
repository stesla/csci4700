#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *func;
  NODE *args;
};

size_t ast_call_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).args, symbols);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_CALL");
  PRINT_EDGE(out, node, S(node).func);
  PRINT_EDGE(out, node, S(node).args);
}

static void set_temps(NODE *node, int val)
{

}

static const char *to_s(NODE *node)
{
  const char *func = ast_to_s(S(node).func);
  const char *args = ast_to_s(S(node).args);
  size_t length = strlen(func) + strlen(args) + 3;
  char *result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s(%s)", func, args);
  return result;
}

void ast_call_init(NODE *node, va_list args)
{
  S(node).func = va_arg(args, NODE *);
  S(node).args = va_arg(args, NODE *);

  SET_METHODS(node);
}
