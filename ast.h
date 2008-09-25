#ifndef AST_H
#define AST_H

typedef struct _node NODE;
typedef enum _node_type {
  AST_ARRAY_LIST,
  AST_CONSTANT,
  AST_IDENTIFIER,
  AST_IDENTIFIER_LIST,
  AST_STRING_LITERAL
} NODE_TYPE;

NODE *ast_create(NODE_TYPE type, ...);
const char *ast_to_s(NODE *node);

#endif AST_H
