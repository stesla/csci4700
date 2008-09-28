#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <string.h>
#include "util.h"

/* To add a new node type, add it to the enum (in alphabetical order). You also
** need to add it to the table in ast_create. You need to provide two
** functions:
**
**    size_t ast_NODE_size() { return SLOT_SIZE; }
**    void ast_NODE_init(NODE *node, va_list args) { ...; SET_METHODS(node); }
**
** The SET_METHODS macro will initialize node's method structure with the pointers
** provided as the rest of the arguments.
**
** Additionally, you should define a structure:
**
**    struct slots {
**      ...
**    };
**
** Then you can access the slots for a specific instance like this:
**
**    S(node).whatever
**
** You can create a new node like this:
**
**    N(NODE_TYPE, ...)
**
** The functions provided at the bottom of this header are wrappers for the
** methods that are available. They are the recommended way of calling the
** methods. For example:
**
**    ast_to_s(node);
*/

typedef enum _node_type {
  AST_ARRAY,
  AST_BINARY,
  AST_BLOCK,
  AST_CALL,
  AST_CONDITIONAL,
  AST_CONSTANT,
  AST_DECLARE,
  AST_FOR,
  AST_FORMAL,
  AST_FUNCTION,
  AST_GROUP,
  AST_IDENTIFIER,
  AST_LIST,
  AST_POSTFIX,
  AST_PREFIX,
  AST_READ,
  AST_RETURN,
  AST_STRING_LITERAL,
  AST_WHILE,
  AST_WRITE
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
  **    Set these using the SET_METHODS macro.
  */
  struct _methods {
    void (*fill_symbols)(NODE *, void *);
    void (*print)(NODE *, FILE *);
    const char *(*to_s)(NODE *);
  } methods;

  /* Slot Table:
  **    Access using the S macro.
  */
  void *slots;
};

#define N ast_create
#define S(n) (*((struct slots *)(n)->slots))
#define SET_METHOD(node, method, func) { (node)->methods.method = (func); }
#define SET_METHODS(node)                           \
  {                                                 \
    SET_METHOD(node, print, print);                 \
    SET_METHOD(node, to_s, to_s);                   \
    SET_METHOD(node, fill_symbols, fill_symbols);   \
  }
#define SLOT_SIZE sizeof(struct slots)

#define PRINT_NODE(out, node, label) {                       \
    fprintf(out, "\"%p\" [label=\"%s\"];\n", node, label);   \
  }
#define PRINT_EDGE(out, tail, head) {                   \
    if (head) {                                         \
      fprintf(out, "\"%p\" -> \"%p\";\n", tail, head);    \
      head->methods.print(head, out);                   \
    }                                                   \
  }

/*
** Public Functions
*/

const char *ast_node_type_str(NODE_TYPE type);
const char *ast_op_str(OP_TYPE type);

NODE *ast_create(NODE_TYPE type, ...);
void ast_fill_symbols(NODE *node, void *symbols);
void ast_print(NODE *node, FILE *out);
const char *ast_to_s(NODE *node);

#endif AST_H
