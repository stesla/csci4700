#ifndef IR_H
#define IR_H

#include <stdio.h>

typedef enum _ir_inst {
  IR_ADD,
  IR_AND,
  IR_ASSIGN,
  IR_ASSIGN_INDIRECT,
  IR_CALL,
  IR_DEREF,
  IR_DIVIDE,
  IR_ENTER,
  IR_IF_EQ,
  IR_IF_FALSE,
  IR_IF_GE,
  IR_IF_GT,
  IR_IF_LE,
  IR_IF_LT,
  IR_IF_NE,
  IR_IF_TRUE,
  IR_JUMP,
  IR_LABEL,
  IR_LEAVE,
  IR_MODULO,
  IR_MULTIPLY,
  IR_OR,
  IR_PARAM,
  IR_POP,
  IR_PUSH,
  IR_READ,
  IR_REF,
  IR_RETURN,
  IR_SUBTRACT,
  IR_WRITE,
  IR_WRITE_LITERAL,
  IR_XOR
} IR_INST;

typedef enum _ir_type {
  IR_CONST,
  IR_LITERAL,
  IR_NULL,
  IR_SYM,
  IR_TEMP
} IR_TYPE;

typedef struct _ir IR;
typedef struct _ir_quad IR_QUAD;
typedef struct _ir_cell IR_CELL;
typedef void (*IR_CALLBACK)(IR_QUAD *, void *);

IR *ir_create();
void ir_add(IR *ir, IR_INST inst, ...);
void ir_each(IR *ir, IR_CALLBACK callback, void *data);
void ir_fprint(FILE *out, IR *ir);
void ir_fprint_quad(FILE *out, IR_QUAD *quad);
int ir_make_label();
int ir_make_temp(IR *ir);
void ir_reset_temp(size_t ar_size);

const char *ir_inst_str(IR_INST inst);
IR_INST ir_quad_inst(IR_QUAD *quad);
IR_CELL *ir_quad_arg1(IR_QUAD *quad);
IR_CELL *ir_quad_arg2(IR_QUAD *quad);
IR_CELL *ir_quad_result(IR_QUAD *quad);
IR_TYPE ir_cell_type(IR_CELL *cell);
void *ir_cell_ptr(IR_CELL *cell);
int ir_cell_num(IR_CELL *cell);

#endif
