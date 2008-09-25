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
  AST_OP_NOT,   // !
  AST_OP_DEC,   // --
  AST_OP_INC,   // ++
  AST_OP_MINUS, // -
  AST_OP_PLUS,  // +
  AST_OP_DEREF, // * (when prefix)
  AST_OP_REF    // & (when prefix)
} OP_TYPE;

NODE *ast_create(NODE_TYPE type, ...);
const char *ast_to_s(NODE *node);

#endif AST_H
