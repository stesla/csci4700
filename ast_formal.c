#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_formal_to_s(NODE *node)
{
  char *result;

  if (S(node).formal.is_array)
    {
      const char *identifier = ast_to_s(S(node).formal.identifier);
      size_t length = strlen(identifier) + 3; /* identifier[] */
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "%s[]", identifier);
    }
  else
    {
      result = strdup(ast_to_s(S(node).formal.identifier));
    }
  return result;
}

void ast_formal_init(NODE *node, va_list args)
{
  S(node).formal.identifier = va_arg(args, NODE *);
  S(node).formal.is_array = va_arg(args, int);

  SET_M(node, ast_formal_to_s);
}
