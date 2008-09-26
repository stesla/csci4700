#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_identifier_to_s(NODE *node)
{
  return S(node).identifier;
}

void ast_identifier_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, char *);

  SET_M(node, ast_identifier_to_s);
}
