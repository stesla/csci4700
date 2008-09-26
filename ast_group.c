#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_group_to_s(NODE *node)
{
  const char *group = ast_to_s(S(node).group);
  size_t length = strlen(group) + 3;
  char *result = my_malloc(length * sizeof(char));
  snprintf(result, length, "(%s)", group);
  return result;
}

void ast_group_init(NODE *node, va_list args)
{
  S(node).group = va_arg(args, NODE *);

  SET_M(node, ast_group_to_s);
}
