#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *first;
  NODE *rest;
};

size_t ast_list_size() { return SLOT_SIZE; }

static void add_symbols(NODE *node, void *symbols)
{
  if (S(node).first)
    ast_add_symbols(S(node).first, symbols);
  if (S(node).rest)
    ast_add_symbols(S(node).rest, symbols);
}

static void find_symbols(NODE *node, void *symbols)
{
  if (S(node).first)
    ast_find_symbols(S(node).first, symbols);
  if (S(node).rest)
    ast_find_symbols(S(node).rest, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  ast_generate_ir(S(node).first, ir);
  if(S(node).rest)
    ast_generate_ir(S(node).rest, ir);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_LIST");

  PRINT_EDGE(out, node, S(node).first);
  PRINT_EDGE(out, node, S(node).rest);
}

static void set_temps(NODE *node, int val)
{
  /* TODO: Should I be using a different temp for first and rest, I don't
   * *think* so, but it may need to be that way. */
  ast_set_temps(S(node).first, val);
  if(S(node).rest)
    ast_set_temps(S(node).rest, val);
}

static const char *to_s(NODE *node)
{
  char *result;

  if(S(node).first == NULL)
    {
      result = "(empty)"; /* empty list */
    }
  else if (S(node).rest == NULL)
    {
      /* Make our own copy of our child's string */
      result = strdup(ast_to_s(S(node).first));
    }
  else
    {
      const char *first = ast_to_s(S(node).first);
      const char *rest = ast_to_s(S(node).rest);
      size_t length = strlen(first) + strlen(rest) + 3;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "%s, %s", first, rest);
    }
  return result;
}

void ast_list_init(NODE *node, va_list args)
{
  S(node).first = va_arg(args, NODE *);
  S(node).rest = va_arg(args, NODE *);

  SET_METHODS(node);
  OVERRIDE(node, add_symbols);
}
