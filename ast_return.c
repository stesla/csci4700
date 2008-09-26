#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *value;
};

static const char *ast_return_to_s(NODE *node)
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
  ALLOC_S(node);

  S(node).value = va_arg(args, NODE *);

  SET_M(node, ast_return_to_s);
}
