#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ir.h"
#include "buffer.h"
#include "literal.h"
#include "sizes.h"
#include "symbol.h"
#include "util.h"

struct _ir_cell {
  IR_TYPE type;
  union {
    void *ptr;
    int num;
  } val;
};

struct _ir_quad {
  IR_INST inst;
  IR_CELL arg1;
  IR_CELL arg2;
  IR_CELL result;
};

struct _ir {
  BUFFER *buffer;
};

/*
** Utility Functions
*/

static void ir_arg(va_list *args, IR_CELL *cell)
{
  int *intp;

  cell->type = va_arg(*args, IR_TYPE);
  switch (cell->type)
    {
    case IR_LITERAL:
    case IR_SYM:
      cell->val.ptr = va_arg(*args, void *);
      break;

    case IR_CONST:
    case IR_TEMP:
      intp = va_arg(*args, int *);
      cell->val.num = *intp;
      break;

    case IR_NULL:
      break;
    }
}

static void ir_fprint_cell(FILE *out, IR_CELL *cell)
{
  switch (cell->type)
    {
    case IR_CONST:
      fprintf(out, "%i", cell->val.num);
      break;
    case IR_LITERAL:
      {
        const char *id = literal_value(cell->val.ptr);
        int address = literal_address(cell->val.ptr);
        fprintf(out, "(:literal \"%s\" L%i)", id, address);
      }
      break;
    case IR_NULL:
      fprintf(out, "nil");
      break;
    case IR_SYM:
      {
        const char *id = symbol_id(cell->val.ptr);
        int address = symbol_address(cell->val.ptr);
        if (symbol_is_global(cell->val.ptr))
          fprintf(out, "(:global \"%s\" L%i)", id, address);
        else
          fprintf(out, "(:local \"%s\" %i)", id, address);
      }
      break;
    case IR_TEMP:
      fprintf(out, "'(:temp %i)", cell->val.num);
      break;
    }
}

void ir_fprint_quad(FILE *out, IR_QUAD *quad)
{
  int i;

  fprintf(out, "(:%s ", ir_inst_str(quad->inst));
  ir_fprint_cell(out, &quad->arg1);
  fprintf(out, " ");
  ir_fprint_cell(out, &quad->arg2);
  fprintf(out, " ");
  ir_fprint_cell(out, &quad->result);
  fprintf(out, ")\n");
}

/*
** Private Functions
*/

static void ir_add_inst(IR *ir, IR_QUAD quad)
{
  IR_QUAD *current = buffer_next(ir->buffer);

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
  IR *result = my_malloc(sizeof(IR));
  result->buffer = buffer_create(sizeof(IR_QUAD), 128);
  return result;
}

void ir_add(IR *ir, IR_INST inst, ...)
{
  IR_QUAD quad = {inst, {IR_NULL,0}, {IR_NULL,0}, {IR_NULL,0}};
  va_list args;

  va_start(args, inst);
  switch (inst)
    {
    case IR_ADD:
    case IR_AND:
    case IR_DIVIDE:
    case IR_IF_EQ:
    case IR_IF_GE:
    case IR_IF_GT:
    case IR_IF_LE:
    case IR_IF_LT:
    case IR_IF_NE:
    case IR_MODULO:
    case IR_MULTIPLY:
    case IR_OR:
    case IR_SUBTRACT:
    case IR_XOR:
      ir_arg(&args, &quad.arg1);
      ir_arg(&args, &quad.arg2);
      ir_arg(&args, &quad.result);
      break;

    case IR_ASSIGN:
    case IR_ASSIGN_INDIRECT:
    case IR_CALL:
    case IR_DEREF:
    case IR_IF_FALSE:
    case IR_REF:
    case IR_IF_TRUE:
      ir_arg(&args, &quad.arg1);
      ir_arg(&args, &quad.result);
      break;

    case IR_JUMP:
    case IR_LABEL:
    case IR_PARAM:
    case IR_POP:
    case IR_PUSH:
    case IR_RETURN:
    case IR_WRITE:
    case IR_WRITE_LITERAL:
      ir_arg(&args, &quad.arg1);
      break;

    case IR_READ:
      ir_arg(&args, &quad.result);
      break;

    case IR_ENTER:
    case IR_HALT:
    case IR_LEAVE:
    case IR_PARAM_START:
      break;
    }
  va_end(args);

  ir_add_inst(ir, quad);
}

void ir_each(IR *ir, IR_CALLBACK callback, void *data)
{
  buffer_each(ir->buffer, (BUFFER_CALLBACK) callback, data);
}

static void ir_fprint_callback(IR_QUAD *quad, void *data)
{
  ir_fprint_quad((FILE *) data, quad);
}

void ir_fprint(FILE *out, IR *ir)
{
  fprintf(out, "'(\n");
  ir_each(ir, ir_fprint_callback, (void *) out);
  fprintf(out, ")");
}

const char *ir_inst_str(IR_INST inst)
{
  static const char *strs[] = {
    "ADD",
    "AND",
    "ASSIGN",
    "ASSIGN_INDIRECT",
    "CALL",
    "DEREF",
    "DIVIDE",
    "ENTER",
    "HALT",
    "IF_EQ",
    "IF_FALSE",
    "IF_GE",
    "IF_GT",
    "IF_LE",
    "IF_LT",
    "IF_NE",
    "IF_TRUE",
    "JUMP",
    "LABEL",
    "LEAVE",
    "MODULO",
    "MULTIPLY",
    "OR",
    "PARAM",
    "PARAM_START",
    "POP",
    "PUSH",
    "READ",
    "REF",
    "RETURN",
    "SUBTRACT",
    "WRITE",
    "WRITE_LITERAL",
    "XOR"
  };
  return strs[inst];
}

int ir_make_label()
{
  static ir_label_ctr = 0;
  return ir_label_ctr++;
}

static int *ir_init_temp()
{
  static int ir_temp_ctr = 0;
  return &ir_temp_ctr;
}

int ir_make_temp(IR *ir)
{
  static int *ctr = NULL;
  size_t size = INTEGER_SIZE;
  if (ctr == NULL)
    ctr = ir_init_temp();
  ir_add(ir, IR_PUSH, IR_CONST, &size);
  return (*ctr) -= INTEGER_SIZE;
}

void ir_reset_temp(size_t ar_size)
{
  static int *ctr = NULL;
  if (ctr == NULL)
    ctr = ir_init_temp();
  *ctr = -ar_size;
}

IR_INST ir_quad_inst(IR_QUAD *quad)
{
  return quad->inst;
}

IR_CELL *ir_quad_arg1(IR_QUAD *quad)
{
  return &(quad->arg1);
}

IR_CELL *ir_quad_arg2(IR_QUAD *quad)
{
  return &(quad->arg2);
}

IR_CELL *ir_quad_result(IR_QUAD *quad)
{
  return &(quad->result);
}

IR_TYPE ir_cell_type(IR_CELL *cell)
{
  return cell->type;
}

void *ir_cell_ptr(IR_CELL *cell)
{
  return cell->val.ptr;
}

int ir_cell_num(IR_CELL *cell)
{
  return cell->val.num;
}

