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

    int constant;

    struct {
      NODE *list;
    } declare;

    struct {
      NODE *identifier;
      int is_array;
    } formal;

    const char *identifier;

    struct {
      NODE *first;
      NODE *rest;
    } list;

    const char *string_literal;
  } stab;
};

/*
** FORWARD DECLARATIONS
*/

/* Array */
static NODE * ast_array_init(NODE *node, va_list args);
static const char *ast_array_to_s(NODE *node);

/* Constant */
static NODE * ast_constant_init(NODE *node, va_list args);
static const char *ast_constant_to_s(NODE *node);

/* Declare */
static NODE * ast_declare_init(NODE *node, va_list args);
static const char *ast_declare_to_s(NODE *node);

/* Formal */
static NODE * ast_formal_init(NODE *node, va_list args);
static const char *ast_formal_to_s(NODE *node);

/* Identifier */
static NODE * ast_identifier_init(NODE *node, va_list args);
static const char *ast_identifier_to_s(NODE *node);

/* Identifier List */
static NODE * ast_list_init(NODE *node, va_list args);
static const char *ast_list_to_s(NODE *node);

/* String Literal */
static NODE * ast_string_literal_init(NODE *node, va_list args);
static const char *ast_string_literal_to_s(NODE *node);

/*
** UTILITIES
*/

static const char *_node_type_str[] =
  {
    "AST_ARRAY",
    "AST_CONSTANT",
    "AST_DECLARE",
    "AST_FORMAL",
    "AST_IDENTIFIER",
    "AST_LIST",
    "AST_STRING_LITERAL"
  };
#define NODE_TYPE_STR(x) (_node_type_str[(x)])

#define M(node) (node->mtab)
#define S(node) (node->stab)

#define SET_M(node,                             \
              _to_s)                            \
  {                                             \
    M(node).to_s = (_to_s);                     \
  }

static NODE *ast_alloc(NODE_TYPE type)
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
  static NODE * (*init[])(NODE *node, va_list args) =
    {
      ast_array_init,
      ast_constant_init,
      ast_declare_init,
      ast_formal_init,
      ast_identifier_init,
      ast_list_init,
      ast_string_literal_init
    };

  va_start(args, type);
  init[type](result, args);
  va_end(args);

  return result;
}

const char *ast_to_s(NODE *node)
{
  return M(node).to_s(node);
}

/*
** PRIVATE FUNCTIONS
*/

/* Array List */
static NODE * ast_array_init(NODE *node, va_list args)
{
  S(node).array.identifier = va_arg(args, NODE *);
  S(node).array.count = va_arg(args, NODE *);

  SET_M(node, ast_array_to_s);

  return node;
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


/* Constant */

static NODE * ast_constant_init(NODE *node, va_list args)
{
  char *text = va_arg(args, char *);
  S(node).constant = atoi(text);
  free(text);

  SET_M(node, ast_constant_to_s);

  return node;
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
static NODE * ast_declare_init(NODE *node, va_list args)
{
  S(node).declare.list = va_arg(args, NODE *);

  SET_M(node, ast_declare_to_s);
}

static const char *ast_declare_to_s(NODE *node)
{
  return ast_to_s(S(node).declare.list);
}

/* Formal */
static NODE * ast_formal_init(NODE *node, va_list args)
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

/* Identifier */

static NODE * ast_identifier_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, char *);

  SET_M(node, ast_identifier_to_s);

  return node;
}

static const char *ast_identifier_to_s(NODE *node)
{
  return S(node).identifier;
}

/* Identifier List */

static NODE *ast_list_init(NODE *node, va_list args)
{
  S(node).list.first = va_arg(args, NODE *);
  S(node).list.rest = va_arg(args, NODE *);

  SET_M(node, ast_list_to_s);

  return node;
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

/* String Literal */

static NODE *ast_string_literal_init(NODE *node, va_list args)
{
  S(node).string_literal = va_arg(args, char *);

  SET_M(node, ast_string_literal_to_s);

  return node;
}

static const char *ast_string_literal_to_s(NODE *node)
{
  return S(node).string_literal;
}
