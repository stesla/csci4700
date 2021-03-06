#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <string.h>
#include "ir.h"
#include "literal.h"
#include "symbol.h"
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
    void (*add_symbols)(NODE *, void *);
    SYMBOL *(*get_symbol)(NODE *);
    void (*find_literals)(NODE *, LITERALS *);
    void (*find_symbols)(NODE *, void *);
    void (*generate_ir)(NODE *, IR *);
    void (*generate_lval_ir)(NODE *, IR *);
    void (*generate_param_ir)(NODE *, IR *);
    void (*check_functions)(NODE *, SYMBOLS *);
    IR_TYPE (*ir_type)(NODE *);
    void *(*ir_value)(NODE *);
    int (*is_lvalue)(NODE *);
    void (*print)(NODE *, FILE *);
    const char *(*to_s)(NODE *);
  } methods;

  /* Slot Table:
  **    Access using the S macro.
  */
  void *slots;
};

#define N ast_create
#define OVERRIDE(node, method) SET_METHOD(node, method, method)
#define S(n) (*((struct slots *)(n)->slots))
#define SET_METHOD(node, method, func) { (node)->methods.method = (func); }
#define SET_METHODS(node)                           \
  {                                                 \
    SET_METHOD(node, find_symbols, find_symbols);   \
    SET_METHOD(node, generate_ir, generate_ir);     \
    SET_METHOD(node, print, print);                 \
    SET_METHOD(node, to_s, to_s);                   \
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
void ast_add_symbols(NODE *node, void *symbols);
void ast_find_literals(NODE *node, LITERALS *literals);
void ast_find_symbols(NODE *node, void *symbols);
SYMBOL *ast_get_symbol(NODE *node);
void ast_generate_ir(NODE *node, IR *ir);
void ast_generate_lval_ir(NODE *node, IR *ir);
void ast_generate_param_ir(NODE *node, IR *ir);
void ast_check_functions(NODE *node, SYMBOLS *symbols);
IR_TYPE ast_ir_type(NODE *node);
void *ast_ir_value(NODE *node);
int ast_is_lvalue(NODE *node);
void ast_print(NODE *node, FILE *out);
OP_TYPE ast_node_type(NODE *node);
const char *ast_to_s(NODE *node);

#endif
