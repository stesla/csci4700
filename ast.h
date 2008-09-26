#ifndef AST_H
#define AST_H

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

/* TODO: Try and hide this again later */
typedef struct _node {
  NODE_TYPE type;
  const char *to_s;

  /* Method Table */
  struct _methods {
    const char *(*to_s)(struct _node *);
  } mtab;

  /* Slot Table:
  **   An entry per NODE_TYPE with the slots for that type.
  */
  union _slots {
    struct {
      struct _node *identifier;
      struct _node *count;
    } array;

    struct {
      OP_TYPE op;
      struct _node *left;
      struct _node *right;
    } binary;

    struct {
      struct _node *func;
      struct _node *args;
    } call;

    int constant;

    struct {
      struct _node *list;
    } declare;

    struct {
      struct _node *identifier;
      int is_array;
    } formal;

    struct _node *group;

    const char *identifier;

    struct {
      struct _node *first;
      struct _node *rest;
    } list;

    struct {
      struct _node *operand;
      OP_TYPE op;
    } unary;

    struct _node *retval;

    const char *string_literal;
  } stab;
} NODE;

NODE *ast_create(NODE_TYPE type, ...);
const char *ast_op_str(OP_TYPE type);
const char *ast_to_s(NODE *node);

#define M(node) (node->mtab)
#define S(node) (node->stab)

#define SET_M(node,                             \
              _to_s)                            \
  {                                             \
    M(node).to_s = (_to_s);                     \
  }

#endif AST_H
