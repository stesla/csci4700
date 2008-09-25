#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "ast.h"
#include "util.h"

/*
** FORWARD DECLARATIONS
*/

static const char *ast_default_to_s(NODE *node);

/* Constant */
static NODE * ast_constant_init(NODE *node, va_list args);
static const char *ast_constant_to_s(NODE *node);

/* Identifier */
static NODE * ast_identifier_init(NODE *node, va_list args);
static const char *ast_identifier_to_s(NODE *node);

/* Identifier List */
static NODE * ast_identifier_list_init(NODE *node, va_list args);
static const char *ast_identifier_list_to_s(NODE *node);

/* String Literal */
static NODE * ast_string_literal_init(NODE *node, va_list args);
static const char *ast_string_literal_to_s(NODE *node);

/*
** TYPES
*/

typedef struct {
  const char *(*to_s)(NODE *);
} MTAB;

typedef union {
  struct {
    int value;
    const char *to_s;
  } constant;

  const char *identifier;

  struct {
    NODE *first;
    NODE *rest;
    const char *to_s;
  } identifier_list;

  const char *string_literal;
} STAB;

#define M(node) (*((MTAB *)(node)->mtab))
#define S(node) (*((STAB *)(node)->stab))

/*
** UTILITIES
*/

static const char *_node_type_strings[] =
  {"AST_CONSTANT",
   "AST_IDENTIFIER",
   "AST_STRING_LITERAL"};
#define NODE_STR(type) (_node_type_strings[(type)])


static NODE *ast_alloc(NODE_TYPE type)
{
  NODE *result = (NODE *) my_malloc(sizeof(NODE));
  result->type = type;

  result->mtab = my_malloc(sizeof(MTAB));
  bzero(result->mtab, sizeof(MTAB));

  result->stab = my_malloc(sizeof(STAB));
  bzero(result->stab, sizeof(STAB));

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
      ast_constant_init,
      ast_identifier_init,
      ast_identifier_list_init,
      ast_string_literal_init
    };

  va_start(args, type);
  init[type](result, args);
  va_end(args);

  return result;
}

const char *ast_to_s(NODE *node)
{
  static const char *(*to_s[])(NODE *node) =
    {
      ast_constant_to_s,
      ast_identifier_to_s,
      ast_identifier_list_to_s,
      ast_string_literal_to_s
    };

  return to_s[node->type](node);
}

/*
** PRIVATE FUNCTIONS
*/

/* init */

static NODE * ast_constant_init(NODE *node, va_list args)
{
  char *text = va_arg(args, char *);
  S(node).constant.value = atoi(text);
  free(text);

  M(node).to_s = ast_constant_to_s;

  return node;
}

static NODE * ast_identifier_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, char *);
  return node;
}

static NODE *ast_identifier_list_init(NODE *node, va_list args)
{
  NODE *first = va_arg(args, NODE *);
  NODE *second = va_arg(args, NODE *);
  NODE *result = NULL;

  if (first->type == AST_IDENTIFIER) /* First Identifier */
    {
      S(node).identifier_list.first = first;
      result = node;
    }
  else
    {
      S(first).identifier_list.rest = node;
      S(node).identifier_list.first = second;
      result = first;
    }
  fprintf(stderr, "ast_identifier_list_init: %s\n", ast_to_s(result));
  return result;
}

static NODE *ast_string_literal_init(NODE *node, va_list args)
{
  S(node).string_literal = va_arg(args, char *);
  return node;
}

/* to_s */

#define MAX_DIGITS 11 /* 10 digits in a 32-bit number + 1 for trailing \0 */
static const char *ast_constant_to_s(NODE *node)
{
  /* The biggest 32-bit integer is 10 digits wide */
  if (S(node).constant.to_s == NULL)
    {
      S(node).constant.to_s = (char *) my_malloc(MAX_DIGITS * sizeof(char));
      snprintf((char *) S(node).constant.to_s, MAX_DIGITS, "%d", S(node).constant.value);
    }
  return S(node).constant.to_s;
}

static const char *ast_identifier_to_s(NODE *node)
{
  return S(node).identifier;
}

static const char *ast_identifier_list_to_s(NODE *node)
{
  char *result;

  /* We can't rely on the previous string value because elements are consed
     onto the back end instead of the front end of the list, and we could be
     reaching this from inside debugging code before the list is complete. */
  if (S(node).identifier_list.to_s)
    free((void *) S(node).identifier_list.to_s);

  if (S(node).identifier_list.rest == NULL)
    {
      /* Make our own copy of our child's string */
      result = strdup(ast_to_s(S(node).identifier_list.first));
    }
  else
    {
      const char *first = ast_to_s(S(node).identifier_list.first);
      const char *rest = ast_to_s(S(node).identifier_list.rest);
      size_t length = strlen(first) + strlen(rest) + 3;
      result = my_malloc(length * sizeof(char));
      snprintf(result, length, "%s, %s", first, rest);
    }

  return S(node).identifier_list.to_s = result;
}

static const char *ast_string_literal_to_s(NODE *node)
{
  return S(node).string_literal;
}
