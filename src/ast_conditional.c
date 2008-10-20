#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  NODE *condition;
  NODE *if_branch;
  NODE *else_branch;
};

size_t ast_conditional_size() { return SLOT_SIZE; }

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).condition, symbols);
  ast_find_symbols(S(node).if_branch, symbols);
  if (S(node).else_branch)
    ast_find_symbols(S(node).else_branch, symbols);
}

static void generate_ir(NODE *node, IR *ir)
{
  /* TODO:IR */
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_CONDITIONAL");

  PRINT_EDGE(out, node, S(node).condition);
  PRINT_EDGE(out, node, S(node).if_branch);
  if (S(node).else_branch)
    PRINT_EDGE(out, node, S(node).else_branch);
}

static void set_temps(NODE *node, int val)
{
  ast_set_temps(S(node).condition, val);
  ast_set_temps(S(node).if_branch, val);
  ast_set_temps(S(node).else_branch, val);
}

static const char *to_s(NODE *node)
{
  char *result;
  size_t length;
  const char *condition = ast_to_s(S(node).condition);

  if(S(node).else_branch == NULL)
    {
      length = strlen(condition) + strlen("IF()") + 1;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "IF(%s)", condition);
    }
  else
    {
      length = strlen(condition) + strlen("IF()ELSE") + 1;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "IF(%s)", condition);
    }

  return result;
}

void ast_conditional_init(NODE *node, va_list args)
{
  S(node).condition = va_arg(args, NODE *);
  S(node).if_branch = va_arg(args, NODE *);
  S(node).else_branch = va_arg(args, NODE *);

  SET_METHODS(node);
}
