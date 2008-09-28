#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "symbol.h"
#include "util.h"

struct slots {
  const char *identifier;
};

size_t ast_identifier_size() { return SLOT_SIZE; }

static void fill_symbols(NODE *node, void *symbols)
{
  symbol_table_add_local(symbols, S(node).identifier);
}

static void print(NODE *node, FILE *out)
{
  size_t length = strlen("AST_IDENTIFIER") + strlen(S(node).identifier) + 3;
  char *label = my_malloc(length * sizeof(char));
  snprintf(label, length, "AST_IDENTIFIER\\n%s", S(node).identifier);

  PRINT_NODE(out, node, label);

  free(label);
}

static const char *to_s(NODE *node)
{
  return S(node).identifier;
}

void ast_identifier_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, char *);

  SET_METHODS(node);
}
