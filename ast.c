#include <stdarg.h>
#include <stdio.h>
#include "ast.h"

static const char *_node_type_strings[] =
  {"AST_CONSTANT",
   "AST_IDENTIFIER",
   "AST_STRING_LITERAL"};
#define NODE_STR(type) (_node_type_strings[(type)])

static NODE *ast_create_identifier(va_list args);
static NODE *ast_create_constant(va_list args);
static NODE *ast_create_string_literal(va_list args);

NODE *ast_create(NODE_TYPE type, ...)
{
  va_list args;
  NODE *result = NULL;

  va_start(args, type);
  switch(type) {
  case AST_CONSTANT:
    result = ast_create_constant(args);
    break;

  case AST_IDENTIFIER:
    result = ast_create_identifier(args);
    break;

  case AST_STRING_LITERAL:
    result = ast_create_string_literal(args);
    break;

  default:
    fprintf(stderr, "UNHANDLED CREATE: %s\n", NODE_STR(type));
  }
  va_end(args);

  return result;
}

/* Private Functions */

static NODE *ast_create_constant(va_list args)
{
  const char *text = va_arg(args, char *);
  int value = atoi(text);
  free(text);
  fprintf(stderr, "ast_create_constant: %d\n", value);
  return NULL;
}

static NODE *ast_create_identifier(va_list args)
{
  const char *text = va_arg(args, char *);
  fprintf(stderr, "ast_create_identfier: %s\n", text);
  return NULL;
}

static NODE *ast_create_string_literal(va_list args)
{
  const char *text = va_arg(args, char *);
  fprintf(stderr, "ast_create_string_literal: %s\n", text);
  return NULL;
}
