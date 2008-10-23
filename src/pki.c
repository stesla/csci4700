#include <stdlib.h>
#include "pki.h"
#include "symbol.h"
#include "util.h"

#define REG_MIN 5
#define REG_MAX 13
#define BP 14
#define SP 15

static int *pki_init_reg()
{
  static int reg = REG_MIN; /* 0,1,2,3,4,14,15 are reserved */
  return &reg;
}

static int pki_get_reg()
{
  static int *reg;
  int result;
  if (reg == NULL)
    reg = pki_init_reg();
  result = (*reg)++;
  if (result > REG_MAX)
    {
      fprintf(stderr, "Error: register spillover\n");
      exit(1);
    }
}

static void pki_reset_reg()
{
  static int *reg;
  if (reg == NULL)
    reg = pki_init_reg();
  *reg = REG_MIN;
}

static void pki_reg_op1(FILE *out, const char *op, int result, int arg1, int arg2)
{
  fprintf(out, "\t%s R%i,R%i,R%i\n", op, result, arg1, arg2);
}

static void pki_ldi(FILE *out, int reg, int base, int offset)
{
  pki_reg_op1(out, "LDI", reg, base, offset);
}

static void pki_load(FILE *out, int reg, int label)
{
  fprintf(out, "\tLOAD R%i,L%i\n", reg, label);
}

static void pki_lsc(FILE *out, int reg, int val)
{
  fprintf(out, "\tLSC R%i,%i\n", reg, val);
}

static int pki_const_reg(FILE *out, int num, int load)
{
  switch(num)
    {
    case 0: return 0;
    case 1: return 1;
    case 2: return 2;
    case 4: return 3;
    default:
      {
        int result = pki_get_reg();
        if (load)
          pki_lsc(out, result, num);
        return result;
      }
    }
}

static int pki_stack_reg(FILE *out, int offset, int load)
{
  int reg = pki_get_reg();
  if (load)
    {
      int offset_reg = pki_const_reg(out, offset, TRUE);
      pki_ldi(out, reg, BP, offset_reg);
    }
  return reg;
}

static int pki_symbol_reg(FILE *out, SYMBOL *sym, int load)
{
  if (symbol_is_global(sym))
    {
      int reg = pki_get_reg();
      if (load)
        pki_load(out, reg, symbol_address(sym));
      return reg;
    }
  else
    return pki_stack_reg(out, symbol_address(sym), load);
}

static int pki_reg(FILE *out, IR_CELL *cell, int load)
{
  int result;
  int address;

  switch (ir_cell_type(cell))
    {
    case IR_CONST: return pki_const_reg(out, ir_cell_num(cell), load);
    case IR_NULL: return 0;
    case IR_SYM: return pki_symbol_reg(out, (SYMBOL *) ir_cell_ptr(cell), load);
    case IR_TEMP: return pki_stack_reg(out, ir_cell_num(cell), load);
    }
}

static void pki_store(FILE *out, int reg, int label)
{
  fprintf(out, "\tSTORE R%i,L%i\n", reg, label);
}

static void pki_sdi(FILE *out, int reg, int base, int offset)
{
  pki_reg_op1(out, "SDI", reg, base, offset);
}

static void pki_store_symbol(FILE *out, int reg, SYMBOL *sym)
{
  if (symbol_is_global(sym))
    pki_store(out, reg, symbol_address(sym));
  else
    pki_sdi(out, reg, BP, symbol_address(sym));
}

static void pki_store_result(FILE *out, int reg, IR_CELL *cell)
{
  switch (ir_cell_type(cell))
    {
    case IR_SYM:
      pki_store_symbol(out, reg, (SYMBOL *) ir_cell_ptr(cell));
      break;
    case IR_TEMP:
      {
        int offset = pki_const_reg(out, ir_cell_num(cell), TRUE);
        pki_sdi(out, reg, BP, offset);
      }
      break;
    default:
      ;
    }
}

static void pki_reg_op(FILE *out, const char *op, IR_QUAD *quad)
{
  int arg1 = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int arg2 = pki_reg(out, ir_quad_arg2(quad), TRUE);
  int result = pki_reg(out, ir_quad_result(quad), FALSE);
  pki_reg_op1(out, op, result, arg1, arg2);
  pki_store_result(out, result, ir_quad_result(quad));
}

static void pki_syscall(FILE *out, int r1, int r2)
{
  fprintf(out, "\tSYSCALL R%i,R%i\n", r1, r2);
}

static void pki_enter(FILE *out)
{
  pki_reg_op1(out, "ADD", BP, SP, 0);
}

static void pki_leave(FILE *out)
{
  pki_reg_op1(out, "ADD", SP, BP, 0);
}

static void pki_label(FILE *out, IR_QUAD *quad)
{
  int num = ir_cell_num(ir_quad_arg1(quad));
  fprintf(out, "L%i: ", num);
}

static void pki_generate_callback(IR_QUAD *quad, void *data)
{
  FILE *out = (FILE *) data;

  fprintf(out, "\t//");
  ir_fprint_quad(out, quad);

  switch (ir_quad_inst(quad))
    {
    case IR_ADD:
      pki_reg_op(out, "ADD", quad);
      break;
    case IR_AND:
      pki_reg_op(out, "AND", quad);
      break;
    case IR_ASSIGN:
    case IR_CALL:
      break;
    case IR_DIVIDE:
      pki_reg_op(out, "DIV", quad);
      break;
    case IR_ENTER:
      pki_enter(out);
      break;
    case IR_IF_EQ:
    case IR_IF_FALSE:
    case IR_IF_GE:
    case IR_IF_GT:
    case IR_IF_LE:
    case IR_IF_LT:
    case IR_IF_NE:
    case IR_IF_TRUE:
    case IR_JUMP:
      break;
    case IR_LABEL:
      pki_label(out, quad);
      break;
    case IR_LEAVE:
      pki_leave(out);
      break;
    case IR_MODULO:
      break;
    case IR_MULTIPLY:
      pki_reg_op(out, "MUL", quad);
      break;
    case IR_OR:
      pki_reg_op(out, "OR", quad);
      break;
    case IR_PARAM:
    case IR_POP:
    case IR_PUSH:
    case IR_READ:
    case IR_RETURN:
      break;
    case IR_SUBTRACT:
      pki_reg_op(out, "SUB", quad);
      break;
    case IR_WRITE:
      break;
    case IR_XOR:
      pki_reg_op(out, "XOR", quad);
      break;
    }
  pki_reset_reg();
}

static void pki_generate_epilogue(FILE *out)
{
  pki_syscall(out, 0, 0);
}

static void pki_generate_prologue(FILE *out)
{
  pki_lsc(out, 1, 1);
  pki_lsc(out, 2, 2);
  pki_lsc(out, 3, 4);
}

void pki_generate(FILE *out, IR *ir)
{
  pki_generate_prologue(out);
  ir_each(ir, pki_generate_callback, (void *) out);
  pki_generate_epilogue(out);
}