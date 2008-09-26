#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *first;
  NODE *rest;
};

size_t ast_list_size() { return SLOT_SIZE; }

static const char *ast_list_to_s(NODE *node)
{
  char *result;

  if(S(node).first == NULL)
    {
      result = "(empty)"; /* empty list */
    }
  else if (S(node).rest == NULL)
    {
      /* Make our own copy of our child's string */
      result = strdup(ast_to_s(S(node).first));
    }
  else
    {
      const char *first = ast_to_s(S(node).first);
      const char *rest = ast_to_s(S(node).rest);
      size_t length = strlen(first) + strlen(rest) + 3;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "%s, %s", first, rest);
    }
  return result;
}

void ast_list_init(NODE *node, va_list args)
{
  S(node).first = va_arg(args, NODE *);
  S(node).rest = va_arg(args, NODE *);

  SET_M(node, ast_list_to_s);
}
