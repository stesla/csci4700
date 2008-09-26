#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *condition;
  NODE *if_branch;
  NODE *else_branch;
};

size_t ast_conditional_size() { return SLOT_SIZE; }

static const char *ast_conditional_to_s(NODE *node)
{
  char *result;
  size_t length;
  const char *condition = ast_to_s(S(node).condition);

  if(S(node).else_branch == NULL)
    {
      length = strlen(condition) + strlen("IF()") + 1;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "IF(%s)", condition);
    }
  else
    {
      length = strlen(condition) + strlen("IF()ELSE") + 1;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "IF(%s)", condition);
    }

  return result;
}

void ast_conditional_init(NODE *node, va_list args)
{
  S(node).condition = va_arg(args, NODE *);
  S(node).if_branch = va_arg(args, NODE *);
  S(node).else_branch = va_arg(args, NODE *);

  SET_M(node, ast_conditional_to_s);
}
