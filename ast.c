#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

/*
** Class Definition
*/

struct _node {
  NODE_TYPE type;
  const char *to_s;

  /* Method Table */
  struct _methods {
    const char *(*to_s)(NODE *);
  } mtab;

  /* Slot Table:
  **   An entry per NODE_TYPE with the slots for that type.
  */
  union _slots {
    struct {
      NODE *identifier;
      NODE *count;
    } array;

    struct {
      OP_TYPE op;
      NODE *left;
      NODE *right;
    } binary;

    struct {
      NODE *func;
      NODE *args;
    } call;

    int constant;

    struct {
      NODE *list;
    } declare;

    struct {
      NODE *identifier;
      int is_array;
    } formal;

    NODE *group;

    const char *identifier;

    struct {
      NODE *first;
      NODE *rest;
    } list;

    struct {
      NODE *operand;
      OP_TYPE op;
    } unary;

    const char *string_literal;
  } stab;
};

/*
** FORWARD DECLARATIONS
*/

/* Array */
static void ast_array_init(NODE *node, va_list args);
static const char *ast_array_to_s(NODE *node);

/* Binary */
static void ast_binary_init(NODE *node, va_list args);
static const char *ast_binary_to_s(NODE *node);

/* Call */
static void ast_call_init(NODE *node, va_list args);
static const char *ast_call_to_s(NODE *node);

/* Constant */
static void ast_constant_init(NODE *node, va_list args);
static const char *ast_constant_to_s(NODE *node);

/* Declare */
static void ast_declare_init(NODE *node, va_list args);
static const char *ast_declare_to_s(NODE *node);

/* Formal */
static void ast_formal_init(NODE *node, va_list args);
static const char *ast_formal_to_s(NODE *node);

/* Group */
static void ast_group_init(NODE *node, va_list args);
static const char *ast_group_to_s(NODE *node);

/* Identifier */
static void ast_identifier_init(NODE *node, va_list args);
static const char *ast_identifier_to_s(NODE *node);

/* List */
static void ast_list_init(NODE *node, va_list args);
static const char *ast_list_to_s(NODE *node);

/* Postfix Op */
static void ast_postfix_init(NODE *node, va_list args);
static const char *ast_postfix_to_s(NODE *node);

/* Prefix Op */
static void ast_prefix_init(NODE *node, va_list args);
static const char *ast_prefix_to_s(NODE *node);

/* String Literal */
static void ast_string_literal_init(NODE *node, va_list args);
static const char *ast_string_literal_to_s(NODE *node);

/*
** UTILITIES
*/

static const char *_node_type_str[] =
  {
    "AST_ARRAY",
    "AST_BINARY",
    "AST_CALL",
    "AST_CONSTANT",
    "AST_DECLARE",
    "AST_FORMAL",
    "AST_GROUP",
    "AST_IDENTIFIER",
    "AST_LIST",
    "AST_POSTFIX",
    "AST_PREFIX",
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
#define OP_TYPE_STR(x) (_op_str[(x)])

#define M(node) (node->mtab)
#define S(node) (node->stab)

#define SET_M(node,                             \
              _to_s)                            \
  {                                             \
    M(node).to_s = (_to_s);                     \
  }

static NODE * ast_alloc(NODE_TYPE type)
{
  NODE *result = (NODE *) my_malloc(sizeof(NODE));
  bzero(result, sizeof(NODE));
  result->type = type;
  return result;
}

/*
** PUBLIC FUNCTIONS
*/

NODE *ast_create(NODE_TYPE type, ...)
{
  va_list args;
  NODE *result = ast_alloc(type);
  static void (*init[])(NODE *node, va_list args) =
    {
      ast_array_init,
      ast_binary_init,
      ast_call_init,
      ast_constant_init,
      ast_declare_init,
      ast_formal_init,
      ast_group_init,
      ast_identifier_init,
      ast_list_init,
      ast_postfix_init,
      ast_prefix_init,
      ast_string_literal_init
    };

  va_start(args, type);
  init[type](result, args);
  va_end(args);

  /* Leave this debug statement in until we're done */
  printf("ast_create(%s): %s\n", NODE_TYPE_STR(result->type), ast_to_s(result));

  return result;
}

const char *ast_to_s(NODE *node)
{
  //TODO: Get rid of this once the AST is completed
  if (node)
    return M(node).to_s(node);
  else
    return "(null)";
}

/*
** PRIVATE FUNCTIONS
*/

/* Array */

static void ast_array_init(NODE *node, va_list args)
{
  S(node).array.identifier = va_arg(args, NODE *);
  S(node).array.count = va_arg(args, NODE *);

  SET_M(node, ast_array_to_s);
}

static const char *ast_array_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      const char *identifier = ast_to_s(S(node).array.identifier);
      const char *count = ast_to_s(S(node).array.count);
      size_t length = strlen(identifier) + strlen(count) + 3; /* first[count] */
      node->to_s = my_malloc(length * sizeof(char));
      snprintf((char *) node->to_s, length, "%s[%s]", identifier, count);
    }
  return node->to_s;
}

/* Binary */

static void ast_binary_init(NODE *node, va_list args)
{
  S(node).binary.op = va_arg(args, OP_TYPE);
  S(node).binary.left = va_arg(args, NODE *);
  S(node).binary.right = va_arg(args, NODE *);

  SET_M(node, ast_binary_to_s);
}

static const char *ast_binary_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      const char *left = ast_to_s(S(node).binary.left);
      const char *op = OP_TYPE_STR(S(node).binary.op);
      const char *right = ast_to_s(S(node).binary.right);
      size_t length = strlen(left) + strlen(op) + strlen(right) + 3;
      node->to_s = my_malloc(length *sizeof(char));
      snprintf((char *) node->to_s, length, "%s %s %s", left, op, right);
    }
  return node->to_s;
}

/* Call */

static void ast_call_init(NODE *node, va_list args)
{
  S(node).call.func = va_arg(args, NODE *);
  S(node).call.args = va_arg(args, NODE *);

  SET_M(node, ast_call_to_s);
}

static const char *ast_call_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      const char *func = ast_to_s(S(node).call.func);
      const char *args = ast_to_s(S(node).call.args);
      size_t length = strlen(func) + strlen(args) + 3;
      node->to_s = malloc(length * sizeof(char));
      snprintf((char *) node->to_s, length, "%s(%s)", func, args);
    }
  return node->to_s;
}

/* Constant */

static void ast_constant_init(NODE *node, va_list args)
{
  char *text = va_arg(args, char *);
  S(node).constant = atoi(text);
  free(text);

  SET_M(node, ast_constant_to_s);
}

#define MAX_DIGITS 11 /* 10 digits in a 32-bit number + 1 for trailing \0 */
static const char *ast_constant_to_s(NODE *node)
{
  /* The biggest 32-bit integer is 10 digits wide */
  if (node->to_s == NULL)
    {
      node->to_s = (char *) my_malloc(MAX_DIGITS * sizeof(char));
      snprintf((char *) node->to_s, MAX_DIGITS, "%d", S(node).constant);
    }
  return node->to_s;
}

/* Declare */
static void ast_declare_init(NODE *node, va_list args)
{
  S(node).declare.list = va_arg(args, NODE *);

  SET_M(node, ast_declare_to_s);
}

static const char *ast_declare_to_s(NODE *node)
{
  return ast_to_s(S(node).declare.list);
}

/* Formal */

static void ast_formal_init(NODE *node, va_list args)
{
  S(node).formal.identifier = va_arg(args, NODE *);
  S(node).formal.is_array = va_arg(args, int);

  SET_M(node, ast_formal_to_s);
}

static const char *ast_formal_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      if (S(node).formal.is_array)
        {
          const char *identifier = ast_to_s(S(node).formal.identifier);
          size_t length = strlen(identifier) + 3; /* identifier[] */
          node->to_s = my_malloc(length * sizeof(char));
          snprintf((char *) node->to_s, length, "%s[]", identifier);
        }
      else
        {
          node->to_s = strdup(ast_to_s(S(node).formal.identifier));
        }
    }
  return node->to_s;
}

/* Group */

static void ast_group_init(NODE *node, va_list args)
{
  S(node).group = va_arg(args, NODE *);

  SET_M(node, ast_group_to_s);
}

static const char *ast_group_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      const char *group = ast_to_s(S(node).group);
      size_t length = strlen(group) + 3;
      node->to_s = my_malloc(length * sizeof(char));
      snprintf((char *) node->to_s, length, "(%s)", group);
    }
  return node->to_s;
}

/* Identifier */

static void ast_identifier_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, char *);

  SET_M(node, ast_identifier_to_s);
}

static const char *ast_identifier_to_s(NODE *node)
{
  return S(node).identifier;
}

/* List */

static void ast_list_init(NODE *node, va_list args)
{
  S(node).list.first = va_arg(args, NODE *);
  S(node).list.rest = va_arg(args, NODE *);

  SET_M(node, ast_list_to_s);
}

static const char *ast_list_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      if(S(node).list.first == NULL)
        {
          node->to_s = "(empty)"; /* empty list */
        }
      else if (S(node).list.rest == NULL)
        {
          /* Make our own copy of our child's string */
          node->to_s = strdup(ast_to_s(S(node).list.first));
        }
      else
        {
          const char *first = ast_to_s(S(node).list.first);
          const char *rest = ast_to_s(S(node).list.rest);
          size_t length = strlen(first) + strlen(rest) + 3;
          node->to_s = my_malloc(length * sizeof(char));
          snprintf((char *) node->to_s, length, "%s, %s", first, rest);
        }
    }

  return node->to_s;
}

/* Postfix Op */

static void ast_postfix_init(NODE *node, va_list args)
{
  S(node).unary.operand = va_arg(args, NODE *);
  S(node).unary.op = va_arg(args, OP_TYPE);

  SET_M(node, ast_postfix_to_s);
}

static const char *ast_postfix_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      const char *operand = ast_to_s(S(node).unary.operand);
      const char *op = OP_TYPE_STR(S(node).unary.op);
      size_t length = strlen(operand) + strlen(op) + 1;
      node->to_s = my_malloc(length * sizeof(char));
      snprintf((char *) node->to_s, length, "%s%s", operand, op);
    }
  return node->to_s;
}

/* Prefix Op */

static void ast_prefix_init(NODE *node, va_list args)
{
  S(node).unary.operand = va_arg(args, NODE *);
  S(node).unary.op = va_arg(args, OP_TYPE);

  SET_M(node, ast_prefix_to_s);
}

static const char *ast_prefix_to_s(NODE *node)
{
  if (node->to_s == NULL)
    {
      const char *operand = ast_to_s(S(node).unary.operand);
      const char *op = OP_TYPE_STR(S(node).unary.op);
      size_t length = strlen(operand) + strlen(op) + 1;
      node->to_s = my_malloc(length * sizeof(char));
      snprintf((char *) node->to_s, length, "%s%s", op, operand);
    }
  return node->to_s;
}

/* String Literal */

static void ast_string_literal_init(NODE *node, va_list args)
{
  S(node).string_literal = va_arg(args, char *);

  SET_M(node, ast_string_literal_to_s);
}

static const char *ast_string_literal_to_s(NODE *node)
{
  return S(node).string_literal;
}
