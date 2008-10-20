#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *value;
};

size_t ast_return_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).value, symbols);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_RETURN");

  if(S(node).value)
    PRINT_EDGE(out, node, S(node).value);
}

static void set_temps(NODE *node, int val)
{
  ast_set_temps(S(node).value, val);
}

static const char *to_s(NODE *node)
{
  char *result;

  if(S(node).value)
    {
      const char *value = ast_to_s(S(node).value);
      size_t length = strlen("RETURN") + strlen(value) + 2;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "RETURN %s", value);
    }
  else
    {
      result = strdup("RETURN");
    }
  return result;
}

void ast_return_init(NODE *node, va_list args)
{
  S(node).value = va_arg(args, NODE *);

  SET_METHODS(node);
}
