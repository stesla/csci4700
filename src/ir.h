#ifndef IR_H
#define IR_H

typedef enum _ir_inst {
  IR_ADD,
  IR_AND,
  IR_ASSIGN,
  IR_CALL,
  IR_DIVIDE,
  IR_IF_FALSE,
  IR_IF_TRUE,
  IR_JUMP,
  IR_LABEL,
  IR_MODULO,
  IR_MULTIPLY,
  IR_OR,
  IR_PARAM,
  IR_READ,
  IR_RETURN,
  IR_SUBTRACT,
  IR_WRITE,
  IR_XOR
} IR_INST;

typedef enum _ir_type {
  IR_CONST,
  IR_NULL,
  IR_SYM,
  IR_TEMP
} IR_TYPE;

typedef struct _ir IR;

IR *ir_create();
void ir_add(IR *ir, IR_INST inst, ...);
void ir_fprint(FILE *out, IR *ir);

#endif
