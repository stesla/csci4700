#ifndef IR_H
#define IR_H

typedef enum _ir_inst {
  IR_ADD,
  IR_AND,
  IR_CALL,
  IR_DIVIDE,
  IR_IF_FALSE,
  IR_IF_TRUE,
  IR_LOAD,
  IR_LOAD_INDIRECT,
  IR_LOAD_LONG,
  IR_LOAD_SHORT,
  IR_MULTIPLY,
  IR_OR,
  IR_RETURN,
  IR_STORE,
  IR_STORE_INDIRECT,
  IR_SUBTRACT,
  IR_SYSCALL,
  IR_XOR
} IR_INSTRUCTION;

typedef enum _ir_type {
  IR_INST,
  IR_CONST,
  IR_REG,
  IR_NULL,
  IR_SYM,
  IR_TEMP
} IR_TYPE;

typedef struct _ir IR;

IR *ir_create();

void ir_add(IR *ir, IR_INSTRUCTION inst, ...);

void ir_add_prologue(IR *ir);

#endif
