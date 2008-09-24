#ifndef AST_H
#define AST_H

typedef struct _node {
  void *data;
} NODE;

typedef enum _node_type {
  AST_CONSTANT,
  AST_IDENTIFIER,
  AST_STRING_LITERAL
} NODE_TYPE;

NODE *ast_create(NODE_TYPE type, ...);

#endif AST_H
