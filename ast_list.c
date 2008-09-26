#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_list_to_s(NODE *node)
{
  char *result;

  if(S(node).list.first == NULL)
    {
      result = "(empty)"; /* empty list */
    }
  else if (S(node).list.rest == NULL)
    {
      /* Make our own copy of our child's string */
      result = strdup(ast_to_s(S(node).list.first));
    }
  else
    {
      const char *first = ast_to_s(S(node).list.first);
      const char *rest = ast_to_s(S(node).list.rest);
      size_t length = strlen(first) + strlen(rest) + 3;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "%s, %s", first, rest);
    }
  return result;
}

void ast_list_init(NODE *node, va_list args)
{
  S(node).list.first = va_arg(args, NODE *);
  S(node).list.rest = va_arg(args, NODE *);

  SET_M(node, ast_list_to_s);
}
