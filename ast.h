#ifndef AST_H
#define AST_H

typedef struct _node NODE;
typedef enum _node_type {
  AST_ARRAY,
  AST_CONSTANT,
  AST_DECLARE,
  AST_FORMAL,
  AST_IDENTIFIER,
  AST_LIST,
  AST_POSTFIX,
  AST_PREFIX,
  AST_STRING_LITERAL
} NODE_TYPE;

typedef enum _op_type {
  AST_OP_AMP,
  AST_OP_BANG,
  AST_OP_MINUS,
  AST_OP_MINUS_MINUS,
  AST_OP_PLUS,
  AST_OP_PLUS_PLUS,
  AST_OP_STAR
} OP_TYPE;

NODE *ast_create(NODE_TYPE type, ...);
const char *ast_to_s(NODE *node);

#endif AST_H
