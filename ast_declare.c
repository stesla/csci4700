#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

static const char *ast_declare_to_s(NODE *node)
{
  return ast_to_s(S(node).declare.list);
}

void ast_declare_init(NODE *node, va_list args)
{
  S(node).declare.list = va_arg(args, NODE *);

  SET_M(node, ast_declare_to_s);
}
