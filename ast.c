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

  /* Method Table */
  struct _methods {
    const char *(*to_s)(NODE *);
  } mtab;

  /* Slot Table:
  **   An entry per NODE_TYPE with the slots for that type.
  */
  union _slots {
    struct {
      NODE *first_identifier;
      NODE *first_count;
      NODE *rest;
      const char *to_s;
    } array_list;

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
  } stab;
};

/*
** FORWARD DECLARATIONS
*/

/* Array List */
static NODE * ast_array_list_init(NODE *node, va_list args);
static const char *ast_array_list_to_s(NODE *node);

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
** UTILITIES
*/

#define M(node) (node->mtab)
#define S(node) (node->stab)

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
      ast_array_list_init,
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
      ast_array_list_to_s,
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

/* Array List */
static NODE * ast_array_list_init(NODE *node, va_list args)
{
  S(node).array_list.first_identifier = va_arg(args, NODE *);
  S(node).array_list.first_count = va_arg(args, NODE *);
  S(node).array_list.rest = va_arg(args, NODE *);
  fprintf(stderr, "array list: %s\n", ast_to_s(node));
  return node;
}

static const char *ast_array_list_to_s(NODE *node)
{
  /* This just makes the code shorter below */
  const char **result = &(S(node).array_list.to_s);

  if (*result == NULL)
    {
      if (S(node).array_list.rest == NULL)
        {
          /* Make our own copy of our child's string */
          const char *identifier = ast_to_s(S(node).array_list.first_identifier);
          const char *count = ast_to_s(S(node).array_list.first_count);
          size_t length = strlen(identifier) + strlen(count) + 3; /* first[count] */
          *result = my_malloc(length * sizeof(char));
          snprintf((char *) *result, length, "%s[%s]", identifier, count);
        }
      else
        {
          const char *identifier = ast_to_s(S(node).array_list.first_identifier);
          const char *count = ast_to_s(S(node).array_list.first_count);
          const char *rest = ast_to_s(S(node).array_list.rest);
          size_t length = strlen(identifier) + strlen(count)+ strlen(rest) + 5; /* first[count], rest */
          *result = my_malloc(length * sizeof(char));
          snprintf((char *) *result, length, "%s[%s], %s", identifier, count, rest);
        }
    }

  return *result;
}

/* Constant */

static NODE * ast_constant_init(NODE *node, va_list args)
{
  char *text = va_arg(args, char *);
  S(node).constant.value = atoi(text);
  free(text);

  M(node).to_s = ast_constant_to_s;

  return node;
}

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

/* Identifier */

static NODE * ast_identifier_init(NODE *node, va_list args)
{
  S(node).identifier = va_arg(args, char *);
  return node;
}

static const char *ast_identifier_to_s(NODE *node)
{
  return S(node).identifier;
}

/* Identifier List */

static NODE *ast_identifier_list_init(NODE *node, va_list args)
{
  S(node).identifier_list.first = va_arg(args, NODE *);
  S(node).identifier_list.rest = va_arg(args, NODE *);
  fprintf(stderr, "id list: %s\n", ast_to_s(node));
  return node;
}

static const char *ast_identifier_list_to_s(NODE *node)
{
  /* This just makes the code shorter below */
  const char **result = &(S(node).identifier_list.to_s);

  if (*result == NULL)
    {
      if (S(node).identifier_list.rest == NULL)
        {
          /* Make our own copy of our child's string */
          *result = strdup(ast_to_s(S(node).identifier_list.first));
        }
      else
        {
          const char *first = ast_to_s(S(node).identifier_list.first);
          const char *rest = ast_to_s(S(node).identifier_list.rest);
          size_t length = strlen(first) + strlen(rest) + 3;
          *result = my_malloc(length * sizeof(char));
          snprintf((char *) *result, length, "%s, %s", first, rest);
        }
    }

  return *result;
}

/* String Literal */

static NODE *ast_string_literal_init(NODE *node, va_list args)
{
  S(node).string_literal = va_arg(args, char *);
  return node;
}

static const char *ast_string_literal_to_s(NODE *node)
{
  return S(node).string_literal;
}
