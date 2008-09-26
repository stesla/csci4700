#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

#include "ast_array.h"
#include "ast_binary.h"
#include "ast_call.h"
#include "ast_constant.h"
#include "ast_declare.h"
#include "ast_formal.h"
#include "ast_group.h"
#include "ast_identifier.h"
#include "ast_list.h"
#include "ast_postfix.h"
#include "ast_prefix.h"
#include "ast_return.h"
#include "ast_string_literal.h"

static const char *_node_type_str[] =
  {
    "AST_ARRAY",
    "AST_BINARY",
    "AST_CALL",
    "AST_CONSTANT",
    "AST_ECLARE",
    "AST_FORMAL",
    "AST_GROUP",
    "AST_IDENTIFIER",
    "AST_LIST",
    "AST_POSTFIX",
    "AST_PREFIX",
    "AST_RETURN",
    "AST_STRING_LITERAL"
  };
#define NODE_TYPE_STR(x) (_node_type_str[(x)])

static const char *_op_str[] =
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
    "--", //AST_OP_MINUS,
    "%",  //AST_OP_MOD,
    "*",  //AST_OP_MULT,
    "!=", //AST_OP_NE,
    "!",  //AST_OP_NOT,
    "+",  //AST_OP_PLUS,
    "&"   //AST_OP_REF
  };

static NODE * ast_alloc(NODE_TYPE type)
{
  NODE *result = (NODE *) my_malloc(sizeof(NODE));
  bzero(result, sizeof(NODE));
  result->type = type;
  return result;
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
      {ast_call_size, ast_call_init},
      {ast_constant_size, ast_constant_init},
      {ast_declare_size, ast_declare_init},
      {ast_formal_size, ast_formal_init},
      {ast_group_size, ast_group_init},
      {ast_identifier_size, ast_identifier_init},
      {ast_list_size, ast_list_init},
      {ast_postfix_size, ast_postfix_init},
      {ast_prefix_size, ast_prefix_init},
      {ast_return_size, ast_return_init},
      {ast_string_literal_size, ast_string_literal_init}
    };

  va_list args;
  NODE *result = ast_alloc(type);
  size_t size = constructor[type].size();

  result->slots = my_malloc(size);
  bzero(result->slots, size);

  va_start(args, type);
  constructor[type].init(result, args);
  va_end(args);

  /* Leave this debug statement in until we're done */
  printf("ast_create(%s): %s\n", NODE_TYPE_STR(result->type), ast_to_s(result));

  return result;
}

const char *ast_op_str(OP_TYPE type)
{
  return _op_str[type];
}

const char *ast_to_s(NODE *node)
{
  //TODO: Get rid of this once the AST is completed
  if (!node)
    return "(null)";

  if (node->to_s == NULL)
    node->to_s = node->methods.to_s(node);

  return node->to_s;
}
