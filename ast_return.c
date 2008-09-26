#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_return_to_s(NODE *node)
{
  char *result;

  if(S(node).retval)
    {
      const char *retval = ast_to_s(S(node).retval);
      size_t length = strlen("RETURN") + strlen(retval) + 2;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "RETURN %s", retval);
    }
  else
    {
      result = strdup("RETURN");
    }
  return result;
}

void ast_return_init(NODE *node, va_list args)
{
  S(node).retval = va_arg(args, NODE *);

  SET_M(node, ast_return_to_s);
}
