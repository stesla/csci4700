#ifndef AST_H
#define AST_H

#include <string.h>
#include "util.h"

typedef enum _node_type {
  AST_ARRAY,
  AST_BINARY,
  AST_CALL,
  AST_CONSTANT,
  AST_DECLARE,
  AST_FORMAL,
  AST_GROUP,
  AST_IDENTIFIER,
  AST_LIST,
  AST_POSTFIX,
  AST_PREFIX,
  AST_RETURN,
  AST_STRING_LITERAL
} NODE_TYPE;

typedef enum _op_type {
  AST_OP_ASSIGN,
  AST_OP_BAND,
  AST_OP_BOR,
  AST_OP_BXOR,
  AST_OP_DEC,
  AST_OP_DEREF,
  AST_OP_DIV,
  AST_OP_EQ,
  AST_OP_GE,
  AST_OP_GT,
  AST_OP_INC,
  AST_OP_LAND,
  AST_OP_LE,
  AST_OP_LOR,
  AST_OP_LT,
  AST_OP_MINUS,
  AST_OP_MOD,
  AST_OP_MULT,
  AST_OP_NE,
  AST_OP_NOT,
  AST_OP_PLUS,
  AST_OP_REF
} OP_TYPE;

typedef struct _node NODE;
struct _node {
  NODE_TYPE type;
  const char *to_s;

  /* Method Table:
  **    Set these using the SET_M macro.
  */
  struct _methods {
    const char *(*to_s)(NODE *);
  } methods;

  /* Slot Table:
  **    Initialize this with the ALLOC_S macro.
  **    Access using the S macro.
  */
  void *slots;
};

NODE *ast_create(NODE_TYPE type, ...);
const char *ast_op_str(OP_TYPE type);
const char *ast_to_s(NODE *node);

#define SET_M(node,                             \
              _to_s)                            \
  {                                             \
    (node)->methods.to_s = (_to_s);             \
  }

#define S(n) (*((struct slots *)(n)->slots))
#define SLOT_SIZE sizeof(struct slots)

#endif AST_H
