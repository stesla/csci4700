#include <stdarg.h>
#include <strings.h>
#include "ast.h"

struct slots {
  const char *value;
};

size_t ast_string_literal_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
}

static void generate_ir(NODE *node, IR *ir)
{
  /* TODO:IR */
}

static void print(NODE *node, FILE *out)
{
  size_t length = strlen("AST_STRING_LITERAL") + strlen(S(node).value) + 7;
  char *label = my_malloc(length * sizeof(char));
  snprintf(label, length, "AST_STRING_LITERAL\\n\\\"%s\\\"", S(node).value);
  PRINT_NODE(out, node, label);

  free(label);
}

static void set_temps(NODE *node, int val)
{
}

static const char *to_s(NODE *node)
{
  return strdup(S(node).value);
}

void ast_string_literal_init(NODE *node, va_list args)
{
  S(node).value = va_arg(args, char *);

  SET_METHODS(node);
}
