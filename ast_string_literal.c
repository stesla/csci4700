#include <stdarg.h>
#include <strings.h>
#include "ast.h"

static const char *ast_string_literal_to_s(NODE *node)
{
  return strdup(S(node).string_literal);
}

void ast_string_literal_init(NODE *node, va_list args)
{
  S(node).string_literal = va_arg(args, char *);

  SET_M(node, ast_string_literal_to_s);
}
