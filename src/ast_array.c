#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include "ast.h"
#include "sizes.h"
#include "symbol.h"
#include "util.h"

struct slots {
  NODE *identifier;
  NODE *count;
  int line;
  SYMBOL *symbol;
  int temp;
};

size_t ast_array_size() { return SLOT_SIZE; }

static void add_symbols(NODE *node, void *symbols)
{
  const char *id = ast_to_s(S(node).identifier);
  size_t count = atoi(ast_to_s(S(node).count));
  symbol_table_add_global_array(symbols, id, count);
  ast_find_symbols(S(node).identifier, symbols);
}

static void generate_index_ir(NODE *node, IR *ir, int result)
{
  int offset = ir_make_temp(ir);
  int base = ir_make_temp(ir);
  SYMBOL *sym = ast_get_symbol(S(node).identifier);
  size_t size = sym ? symbol_size(sym) : INTEGER_SIZE;
  ast_generate_ir(S(node).identifier, ir);
  ast_generate_ir(S(node).count, ir);
  ir_add(ir, IR_REF,
         ast_ir_type(S(node).identifier), ast_ir_value(S(node).identifier),
         IR_TEMP, &base);
  ir_add(ir, IR_MULTIPLY,
         ast_ir_type(S(node).count), ast_ir_value(S(node).count),
         IR_CONST, &size,
         IR_TEMP, &offset);
  ir_add(ir, IR_ADD, IR_TEMP, &base, IR_TEMP, &offset, IR_TEMP, &result);
}

static void generate_ir(NODE *node, IR *ir)
{
  int index = ir_make_temp(ir);
  S(node).temp = ir_make_temp(ir);
  generate_index_ir(node, ir, index);
  ir_add(ir, IR_DEREF, IR_TEMP, &index, ast_ir_type(node), ast_ir_value(node));
}

static void generate_lval_ir(NODE *node, IR *ir)
{
  S(node).temp = ir_make_temp(ir);
  generate_index_ir(node, ir, S(node).temp);
}

static void find_symbols(NODE *node, void *symbols)
{
  ast_find_symbols(S(node).identifier, symbols);
  ast_find_symbols(S(node).count, symbols);
}

static IR_TYPE ir_type(NODE *node)
{
  return IR_TEMP;
}

static void *ir_value(NODE *node)
{
  return &(S(node).temp);
}

static void print(NODE *node, FILE *out)
{
  PRINT_NODE(out, node, "AST_ARRAY");
  PRINT_EDGE(out, node, S(node).identifier);
  PRINT_EDGE(out, node, S(node).count);
}

static const char *to_s(NODE *node)
{
  const char *identifier = ast_to_s(S(node).identifier);
  const char *count = ast_to_s(S(node).count);
  size_t length = strlen(identifier) + strlen(count) + 3; /* first[count] */
  char *result = my_malloc(length * sizeof(char));
  snprintf(result, length, "%s[%s]", identifier, count);
  return result;
}

void ast_array_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, NODE *);
  S(node).count = va_arg(args, NODE *);
  S(node).line = va_arg(args, int);

  SET_METHODS(node);
  OVERRIDE(node, add_symbols);
  OVERRIDE(node, generate_lval_ir);
  OVERRIDE(node, ir_type);
  OVERRIDE(node, ir_value);
}
