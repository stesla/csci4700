#include <stdarg.h>
#include <strings.h>
#include "ast.h"

struct slots {
  const char *value;
};

static const char *ast_string_literal_to_s(NODE *node)
{
  return strdup(S(node).value);
}

void ast_string_literal_init(NODE *node, va_list args)
{
  ALLOC_S(node);

  S(node).value = va_arg(args, char *);

  SET_M(node, ast_string_literal_to_s);
}
