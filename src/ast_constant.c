#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

struct slots {
  int value;
  int temp;
};

size_t ast_constant_size() { return SLOT_SIZE; }

static int get_temp(NODE *node)
{
  return S(node).temp;
}

static void find_symbols(NODE *node, void *symbols)
{
}

static void generate_ir(NODE *node, IR *ir)
{
  ir_add(ir, IR_ASSIGN, IR_CONST, S(node).value, IR_TEMP, S(node).temp);
}

static void print(NODE *node, FILE *out)
{
  char label[25]; /* AST_CONSTANT 1234567890\0 */
  snprintf(label, 24, "AST_CONSTANT\\n%d", S(node).value);
  PRINT_NODE(out, node, label);
}

#define MAX_DIGITS 11 /* 10 digits in a 32-bit number + 1 for trailing \0 */
static const char *to_s(NODE *node)
{
  char *result = (char *) my_malloc(MAX_DIGITS * sizeof(char));
  snprintf(result, MAX_DIGITS, "%d", S(node).value);
  return result;
}

void ast_constant_init(NODE *node, va_list args)
{
  char *text = va_arg(args, char *);

  S(node).value = atoi(text);
  free(text);

  SET_METHODS(node);
  OVERRIDE(node, get_temp);
}
