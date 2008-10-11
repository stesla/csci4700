#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "ir.h"
#include "symbol.h"
#include "util.h"

typedef struct _ir_cell {
  IR_TYPE type;
  union {
    void *ptr;
    int num;
  } val;
} CELL;

typedef struct _ir_quad {
  IR_INST inst;
  CELL arg1;
  CELL arg2;
  CELL result;
} QUAD;

/* ir.h: typedef struct _ir IR; */
struct _ir {
  QUAD *start;
  QUAD *point;
  size_t size;
};

#define GROW_BY (128 * sizeof(QUAD))

/*
** Utility Functions
*/

static void ir_arg(va_list *args, CELL *cell)
{
  cell->type = va_arg(*args, IR_TYPE);
  switch (cell->type)
    {
    case IR_SYM:
      cell->val.ptr = va_arg(*args, void *);
      break;

    case IR_CONST:
    case IR_TEMP:
      cell->val.num = va_arg(*args, int);
      break;

    case IR_NULL:
      break;
    }
}

static void ir_fprint_cell(FILE *out, CELL *cell)
{
  const char *id;

  switch (cell->type)
    {
    case IR_CONST:
      fprintf(out, "%i", cell->val.num);
      break;

    case IR_NULL:
      fprintf(out, "nil");
      break;

    case IR_SYM:
      id = symbol_id(cell->val.ptr);
      fprintf(out, "id:%s:%p", id, cell->val.ptr);
      break;

    case IR_TEMP:
      fprintf(out, "temp:%i", cell->val.num);
      break;
    }
}

static void ir_fprint_quad(FILE *out, QUAD *quad)
{
  static const char *inst[] = {
    "ADD",
    "AND",
    "ASSIGN",
    "CALL",
    "DIVIDE",
    "IF-FALSE",
    "IF-TRUE",
    "JUMP",
    "LABEL",
    "MULTIPLY",
    "OR",
    "PARAM",
    "READ",
    "RETURN",
    "SUBTRACT",
    "WRITE",
    "XOR"
  };
  int i;

  fprintf(out, "(%s, ", inst[quad->inst]);
  ir_fprint_cell(out, &quad->arg1);
  fprintf(out, ", ");
  ir_fprint_cell(out, &quad->arg2);
  fprintf(out, ", ");
  ir_fprint_cell(out, &quad->result);
  fprintf(out, ")\n");
}

void ir_grow(IR *ir)
{
  ir->start = my_realloc(ir->start, GROW_BY);
  ir->point = ir->start + ir->size;
  ir->size = ir->size + GROW_BY;
}

int ir_should_grow(IR *ir)
{
  return (ir->start + ir->size) == ir->point;
}

/*
** Private Functions
*/

static void ir_add_inst(IR *ir, QUAD quad)
{
  QUAD *current;

  if (ir_should_grow(ir))
    ir_grow(ir);

  current = ir->point++;

  current->inst = quad.inst;
  current->arg1 = quad.arg1;
  current->arg2 = quad.arg2;
  current->result = quad.result;
}

/*
** Public Functions
*/

IR *ir_create()
{
  return my_malloc(sizeof(IR));
}

void ir_add(IR *ir, IR_INST inst, ...)
{
  QUAD quad = {inst, {IR_NULL,0}, {IR_NULL,0}, {IR_NULL,0}};
  va_list args;

  va_start(args, inst);
  switch (inst)
    {
    case IR_ADD:
    case IR_AND:
    case IR_DIVIDE:
    case IR_MULTIPLY:
    case IR_OR:
    case IR_SUBTRACT:
    case IR_XOR:
      ir_arg(&args, &quad.arg1);
      ir_arg(&args, &quad.arg2);
      ir_arg(&args, &quad.result);
      break;

    case IR_ASSIGN:
    case IR_IF_FALSE:
    case IR_IF_TRUE:
      ir_arg(&args, &quad.arg1);
      ir_arg(&args, &quad.result);
      break;

    case IR_CALL:
    case IR_JUMP:
    case IR_LABEL:
    case IR_PARAM:
    case IR_READ:
    case IR_RETURN:
    case IR_WRITE:
      ir_arg(&args, &quad.arg1);
      break;
    }
  va_end(args);

  ir_add_inst(ir, quad);
}

void ir_fprint(FILE *out, IR *ir)
{
  QUAD *current = ir->start;

  while (current < ir->point)
    ir_fprint_quad(out, current++);
}
