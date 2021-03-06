#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "symbol.h"
#include "util.h"

struct slots {
  NODE *identifier;
  int is_array;
  int line;
};

size_t ast_formal_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  const char *id = ast_to_s(S(node).identifier);
  symbol_table_add_symbol(symbols, id);
}

static void generate_ir(NODE *node, IR *ir)
{
}

static void print(NODE *node, FILE *out)
{
  char label[17]; /* AST_FORMAL ARRAY */
  snprintf(label, 17, "AST_FORMAL%s", S(node).is_array ? " ARRAY" : "");

  PRINT_NODE(out, node, label);

  PRINT_EDGE(out, node, S(node).identifier);
}

static const char *to_s(NODE *node)
{
  char *result;

  if (S(node).is_array)
    {
      const char *identifier = ast_to_s(S(node).identifier);
      size_t length = strlen(identifier) + 3; /* identifier[] */
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "%s[]", identifier);
    }
  else
    {
      result = strdup(ast_to_s(S(node).identifier));
    }
  return result;
}

void ast_formal_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, NODE *);
  S(node).is_array = va_arg(args, int);
  S(node).line = va_arg(args, int);

  SET_METHODS(node);
}
