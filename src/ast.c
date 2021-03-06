#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

#include "ast_array.h"
#include "ast_binary.h"
#include "ast_block.h"
#include "ast_call.h"
#include "ast_conditional.h"
#include "ast_constant.h"
#include "ast_declare.h"
#include "ast_for.h"
#include "ast_formal.h"
#include "ast_function.h"
#include "ast_group.h"
#include "ast_identifier.h"
#include "ast_list.h"
#include "ast_postfix.h"
#include "ast_prefix.h"
#include "ast_read.h"
#include "ast_return.h"
#include "ast_string_literal.h"
#include "ast_while.h"
#include "ast_write.h"

static NODE * ast_alloc(NODE_TYPE type)
{
  NODE *result = (NODE *) my_malloc(sizeof(NODE));
  result->type = type;
  return result;
}

static void ast_default_add_symbols(NODE *node, void *symbols)
{
}

static void ast_default_find_literals(NODE *node, LITERALS *literals)
{
}

static void ast_default_generate_lval_ir(NODE *node, IR *ir)
{
}

static void ast_default_generate_param_ir(NODE *node, IR *ir)
{
}

static void ast_default_check_functions(NODE *node, SYMBOLS *symbols)
{
}

static IR_TYPE ast_default_ir_type(NODE *node)
{
  return -1;
}

void *ast_default_ir_value(NODE *node)
{
  return NULL;
}

static SYMBOL *ast_default_get_symbol(NODE *node)
{
  return NULL;
}

static int ast_default_is_lvalue(NODE *node)
{
  return FALSE;
}

struct constructor {
  size_t (*size)();
  void (*init)(NODE *node, va_list args);
};

NODE *ast_create(NODE_TYPE type, ...)
{
  static struct constructor constructor[] =
    {
      {ast_array_size, ast_array_init},
      {ast_binary_size, ast_binary_init},
      {ast_block_size, ast_block_init},
      {ast_call_size, ast_call_init},
      {ast_conditional_size, ast_conditional_init},
      {ast_constant_size, ast_constant_init},
      {ast_declare_size, ast_declare_init},
      {ast_for_size, ast_for_init},
      {ast_formal_size, ast_formal_init},
      {ast_function_size, ast_function_init},
      {ast_group_size, ast_group_init},
      {ast_identifier_size, ast_identifier_init},
      {ast_list_size, ast_list_init},
      {ast_postfix_size, ast_postfix_init},
      {ast_prefix_size, ast_prefix_init},
      {ast_read_size, ast_read_init},
      {ast_return_size, ast_return_init},
      {ast_string_literal_size, ast_string_literal_init},
      {ast_while_size, ast_while_init},
      {ast_write_size, ast_write_init}
    };

  va_list args;
  NODE *result = ast_alloc(type);
  size_t size = constructor[type].size();

  result->slots = my_malloc(size);

  SET_METHOD(result, generate_lval_ir, ast_default_generate_lval_ir);
  SET_METHOD(result, generate_param_ir, ast_default_generate_param_ir);
  SET_METHOD(result, find_literals, ast_default_find_literals);
  SET_METHOD(result, get_symbol, ast_default_get_symbol);
  SET_METHOD(result, add_symbols, ast_default_add_symbols);
  SET_METHOD(result, check_functions, ast_default_check_functions);
  SET_METHOD(result, ir_type, ast_default_ir_type);
  SET_METHOD(result, ir_value, ast_default_ir_value);
  SET_METHOD(result, is_lvalue, ast_default_is_lvalue);

  va_start(args, type);
  constructor[type].init(result, args);
  va_end(args);

  return result;
}

void ast_add_symbols(NODE *node, void *symbols)
{
  node->methods.add_symbols(node, symbols);
}

void ast_generate_ir(NODE *node, IR *ir)
{
  node->methods.generate_ir(node, ir);
}

void ast_generate_lval_ir(NODE *node, IR *ir)
{
  node->methods.generate_lval_ir(node, ir);
}

void ast_generate_param_ir(NODE *node, IR *ir)
{
  node->methods.generate_param_ir(node, ir);
}

SYMBOL *ast_get_symbol(NODE *node)
{
  return node->methods.get_symbol(node);
}

void ast_find_literals(NODE *node, LITERALS *literals)
{
  node->methods.find_literals(node, literals);
}

void ast_find_symbols(NODE *node, void *symbols)
{
  node->methods.find_symbols(node, symbols);
}

void ast_check_functions(NODE *node, SYMBOLS *symbols)
{
  node->methods.check_functions(node, symbols);
}

IR_TYPE ast_ir_type(NODE *node)
{
  return node->methods.ir_type(node);
}

void *ast_ir_value(NODE *node)
{
  return node->methods.ir_value(node);
}

int ast_is_lvalue(NODE *node)
{
  return node->methods.is_lvalue(node);
}

const char *ast_node_type_str(NODE_TYPE type)
{
  static const char *table[] =
    {
      "AST_ARRAY",
      "AST_BINARY",
      "AST_BLOCK",
      "AST_CALL",
      "AST_CONDITIONAL",
      "AST_CONSTANT",
      "AST_DECLARE",
      "AST_FOR",
      "AST_FORMAL",
      "AST_FUNCTION",
      "AST_GROUP",
      "AST_IDENTIFIER",
      "AST_LIST",
      "AST_POSTFIX",
      "AST_PREFIX",
      "AST_READ",
      "AST_RETURN",
      "AST_STRING_LITERAL",
      "AST_WHILE",
      "AST_WRITE"
    };
  return table[type];
}

const char *ast_op_str(OP_TYPE type)
{
  static const char *table[] =
    {
      "=",  //AST_OP_ASSIGN,
      "&",  //AST_OP_BAND,
      "|",  //AST_OP_BOR,
      "^",  //AST_OP_BXOR,
      "--", //AST_OP_DEC,
      "*",  //AST_OP_DEREF,
      "/",  //AST_OP_DIV,
      "==", //AST_OP_EQ,
      ">=", //AST_OP_GE,
      ">",  //AST_OP_GT,
      "++", //AST_OP_INC,
      "&&", //AST_OP_LAND,
      "<=", //AST_OP_LE,
      "||", //AST_OP_LOR,
      "<",  //AST_OP_LT,
      "-",  //AST_OP_MINUS,
      "%",  //AST_OP_MOD,
      "*",  //AST_OP_MULT,
      "!=", //AST_OP_NE,
      "!",  //AST_OP_NOT,
      "+",  //AST_OP_PLUS,
      "&"   //AST_OP_REF
    };

  return table[type];
}

void ast_print(NODE *node, FILE *out)
{
  /* We assume that node is not NULL */
  fprintf(out, "digraph AST {\n");
  fprintf(out, "node[shape=box];\n");
  fprintf(out, "edge[style=solid];\n");
  node->methods.print(node, out);
  fprintf(out, "}\n");
}

OP_TYPE ast_node_type(NODE *node)
{
  return node->type;
}

const char *ast_to_s(NODE *node)
{
  if (!node)
    return "(null)";

  if (node->to_s == NULL)
    node->to_s = node->methods.to_s(node);

  return node->to_s;
}
