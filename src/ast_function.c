#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"
#include "symbol.h"

struct slots {
  NODE *identifier;
  NODE *params;
  NODE *body;
  void *symbols;
  int end_line;
};

size_t ast_function_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  void *cur;

  /* TODO */
  /* ast_find_symbols(S(node).identifier, symbols); */
  S(node).symbols = symbol_table_create(symbols);
  ast_find_symbols(S(node).params, S(node).symbols);
  ast_find_symbols(S(node).body, S(node).symbols);

  symbol_table_first(S(node).symbols, &cur);
  while (cur)
    {
      printf("REMOVE %s at line %i\n", symbol_id(cur), S(node).end_line);
      symbol_table_next(&cur);
    }
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_FUNCTION");

  PRINT_EDGE(out, node, S(node).identifier);
  PRINT_EDGE(out, node, S(node).params);
  PRINT_EDGE(out, node, S(node).body);
}

static const char *to_s(NODE *node)
{
  char *result;
  const char *identifier = ast_to_s(S(node).identifier);
  const char *params = ast_to_s(S(node).params);
  size_t length = strlen(identifier) + strlen(params) + 3;
  result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s(%s)", identifier, params);
  return result;
}

void ast_function_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, NODE *);
  S(node).params = va_arg(args, NODE *);
  S(node).body = va_arg(args, NODE *);
  S(node).end_line = va_arg(args, int);

  SET_METHODS(node);
}