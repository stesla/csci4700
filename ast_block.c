#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *declarations;
  NODE *statements;
};

size_t ast_block_size() { return SLOT_SIZE; }

static const char *ast_block_to_s(NODE *node)
{
  const char *result;

  if (S(node).declarations && S(node).statements) { result = "BLOCK {D S}"; }
  else if (!S(node).declarations && !S(node).statements ) { result ="BLOCK {}"; }
  else if (!S(node).declarations) { result = "BLOCK {S}"; }
  else { result = "BLOCK {D}"; }

  return strdup(result);
}

void ast_block_init(NODE *node, va_list args)
{
  S(node).declarations = va_arg(args, NODE *);
  S(node).statements = va_arg(args, NODE *);

  SET_M(node, ast_block_to_s);
}
