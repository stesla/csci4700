#include <stdlib.h>
#include "pki.h"
#include "sizes.h"
#include "symbol.h"
#include "util.h"

#define REG_MIN 5
#define REG_MAX 13
#define BP 14
#define SP 15

enum _syscall {
  PKI_SYSCALL_HALT,
  PKI_SYSCALL_WRITE_REG,
  PKI_SYSCALL_WRITE_STR,
  PKI_SYSCALL_READ
};

static int *pki_init_reg()
{
  static int reg = REG_MIN; /* 0,1,2,3,,14,15 are reserved */
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
  return result;
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

static void pki_llc(FILE *out, int reg, int val)
{
  fprintf(out, "\tLLC R%i,%i\n", reg, val);
}

static void pki_lsc(FILE *out, int reg, int val)
{
  fprintf(out, "\tLSC R%i,%i\n", reg, val);
}

static int pki_const_reg(FILE *out, int num, int load)
{
  if (num >= 0 && num < REG_MIN)
    return num;
  else
    {
      int result = pki_get_reg();
      if (load)
        {
          if (num >= -128 && num <= 127)
            pki_lsc(out, result, num);
          else
            pki_llc(out, result, num);
        }
      return result;
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

static int pki_literal_reg(FILE *out, LITERAL *lit, int load)
{
  int reg = pki_get_reg();
  if (load)
    fprintf(out, "\tLLC R%i,L%i\n", reg, literal_address(lit));
  return reg;
}

static int pki_reg(FILE *out, IR_CELL *cell, int load)
{
  int result;
  int address;

  switch (ir_cell_type(cell))
    {
    case IR_CONST: return pki_const_reg(out, ir_cell_num(cell), load);
    case IR_LITERAL: return pki_literal_reg(out, ir_cell_ptr(cell), load);
    case IR_NULL: return 0;
    case IR_SYM: return pki_symbol_reg(out, (SYMBOL *) ir_cell_ptr(cell), load);
    case IR_TEMP: return pki_stack_reg(out, ir_cell_num(cell), load);
    }
}

static void pki_store(FILE *out, int reg, int label)
{
  fprintf(out, "\tSTORE R%i,L%i\n", reg, label);
}

static void pki_sti(FILE *out, int reg, int base, int offset)
{
  pki_reg_op1(out, "STI", reg, base, offset);
}

static void pki_store_symbol(FILE *out, int reg, SYMBOL *sym)
{
  if (symbol_is_global(sym))
    pki_store(out, reg, symbol_address(sym));
  else
    {
      int offset = pki_const_reg(out, symbol_address(sym), TRUE);
      pki_sti(out, reg, BP, offset);
    }
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
        pki_sti(out, reg, BP, offset);
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

static void pki_label(FILE *out, IR_QUAD *quad)
{
  int num = ir_cell_num(ir_quad_arg1(quad));
  fprintf(out, "L%i: ", num);
}

static void pki_assign(FILE *out, IR_QUAD *quad)
{
  int value = pki_reg(out, ir_quad_arg1(quad), TRUE);
  pki_store_result(out, value, ir_quad_result(quad));
}

static void pki_assign_indirect(FILE *out, IR_QUAD *quad)
{
  int value = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int offset = pki_reg(out, ir_quad_result(quad), TRUE);
  pki_sti(out, value, 0, offset);
}

static void pki_jump(FILE *out, IR_QUAD *quad)
{
  fprintf(out, "\tJUMP L%i\n", ir_cell_num(ir_quad_arg1(quad)));
}

static void pki_push(FILE *out, IR_QUAD *quad)
{
  int reg = pki_reg(out, ir_quad_arg1(quad), TRUE);
  pki_reg_op1(out, "SUB", SP, SP, reg);
}

static void pki_read(FILE *out, IR_QUAD *quad)
{
  int result = pki_reg(out, ir_quad_result(quad), FALSE);
  pki_syscall(out, PKI_SYSCALL_READ, result);
  pki_store_result(out, result, ir_quad_result(quad));
}

static void pki_write(FILE *out, IR_QUAD *quad)
{
  int reg = pki_reg(out, ir_quad_arg1(quad), TRUE);
  pki_syscall(out, PKI_SYSCALL_WRITE_REG, reg);
}

static void pki_write_literal(FILE *out, IR_QUAD *quad)
{
  int reg = pki_reg(out, ir_quad_arg1(quad), TRUE);
  pki_syscall(out, PKI_SYSCALL_WRITE_STR, reg);
}


static void pki_rel_op(FILE *out, const char *rel, int lhs, int rhs, int label)
{
  pki_reg_op1(out, "SUB", 0, lhs, rhs);
  fprintf(out, "\tJUMP %s,L%i\n", rel, label);
}

static void pki_if_rel1(FILE *out, const char *rel, IR_QUAD *quad)
{
  int arg1 = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int arg2 = pki_reg(out, ir_quad_arg2(quad), TRUE);
  int label = ir_cell_num(ir_quad_result(quad));
  pki_rel_op(out, rel, arg1, arg2, label);
}

static void pki_if_rel2(FILE *out, const char *rel, IR_QUAD *quad)
{
  int arg1 = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int arg2 = pki_reg(out, ir_quad_arg2(quad), TRUE);
  int label = ir_cell_num(ir_quad_result(quad));
  pki_rel_op(out, rel, arg2, arg1, label);
}

static void pki_if_false(FILE *out, IR_QUAD *quad)
{
  int arg1 = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int label = ir_cell_num(ir_quad_result(quad));
  pki_reg_op1(out, "SUB", 0, arg1, 0);
  fprintf(out, "\tJUMP EQ,L%i\n", label);
}

static void pki_if_true(FILE *out, IR_QUAD *quad)
{
  int arg1 = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int label = ir_cell_num(ir_quad_result(quad));
  pki_reg_op1(out, "SUB", 0, arg1, 0);
  fprintf(out, "\tJUMP NE,L%i\n", label);
}

static void pki_modulo(FILE *out, IR_QUAD *quad)
{
  int arg1 = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int arg2 = pki_reg(out, ir_quad_arg2(quad), TRUE);
  int result = pki_reg(out, ir_quad_result(quad), FALSE);
  int temp = pki_get_reg();
  pki_reg_op1(out, "DIV", temp, arg1, arg2);
  pki_reg_op1(out, "MUL", temp, temp, arg2);
  pki_reg_op1(out, "SUB", result, arg1, temp);
  pki_store_result(out, result, ir_quad_result(quad));
}

static void pki_ref(FILE *out, IR_QUAD *quad)
{
  SYMBOL *symbol = ir_cell_ptr(ir_quad_arg1(quad));
  int result = pki_reg(out, ir_quad_result(quad), FALSE);
  if (symbol_is_global(symbol))
    fprintf(out, "\tLLC R%i,L%i\n", result, symbol_address(symbol));
  else
    {
      int offset = pki_const_reg(out, symbol_address(symbol), TRUE);
      pki_reg_op1(out, "ADD", result, BP, offset);
    }
  pki_store_result(out, result, ir_quad_result(quad));
}

static void pki_deref(FILE *out, IR_QUAD *quad)
{
  int arg1 = pki_reg(out, ir_quad_arg1(quad), TRUE);
  int result = pki_reg(out, ir_quad_result(quad), FALSE);
  pki_ldi(out, result, 0, arg1);
  pki_store_result(out, result, ir_quad_result(quad));
}

static void pki_enter(FILE *out)
{
  int size = pki_const_reg(out, INTEGER_SIZE, TRUE);
  pki_reg_op1(out, "ADD", BP, SP, 0);
}

static void pki_leave(FILE *out)
{
  int size = pki_const_reg(out, INTEGER_SIZE, TRUE);
  pki_reg_op1(out, "ADD", SP, BP, 0);
}

static void pki_call(FILE *out, IR_QUAD *quad)
{
  SYMBOL *fun = (SYMBOL *) ir_cell_ptr(ir_quad_arg1(quad));
  int size = pki_const_reg(out, INTEGER_SIZE, TRUE);
  pki_reg_op1(out, "SUB", SP, SP, size); /* PUSH */
  pki_sti(out, BP, SP, 0);
  fprintf(out, "\tCALL L%i\n", symbol_address(fun));
  pki_ldi(out, BP, SP, 0);
  pki_reg_op1(out, "ADD", SP, SP, size); /* POP */
}

static void pki_return(FILE *out, IR_QUAD *quad)
{
  /* TODO: get return value out of quad */
  fprintf(out, "\tRETURN\n");
}

static void pki_generate_callback(IR_QUAD *quad, void *data)
{
  FILE *out = (FILE *) data;

  fprintf(out, ";");
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
      pki_assign(out, quad);
      break;
    case IR_ASSIGN_INDIRECT:
      pki_assign_indirect(out, quad);
      break;
    case IR_CALL:
      pki_call(out, quad);
      break;
    case IR_DEREF:
      pki_deref(out, quad);
      break;
    case IR_DIVIDE:
      pki_reg_op(out, "DIV", quad);
      break;
    case IR_ENTER:
      pki_enter(out);
      break;
    case IR_HALT:
      pki_syscall(out, PKI_SYSCALL_HALT, PKI_SYSCALL_HALT);
      break;
    case IR_IF_EQ:
      pki_if_rel1(out, "EQ", quad);
      break;
    case IR_IF_FALSE:
      pki_if_false(out, quad);
      break;
    case IR_IF_GE:
      pki_if_rel2(out, "SLE", quad);
      break;
    case IR_IF_GT:
      pki_if_rel2(out, "SLT", quad);
      break;
    case IR_IF_LE:
      pki_if_rel1(out, "SLE", quad);
      break;
    case IR_IF_LT:
      pki_if_rel1(out, "SLT", quad);
      break;
    case IR_IF_NE:
      pki_if_rel1(out, "NE", quad);
      break;
    case IR_IF_TRUE:
      pki_if_true(out, quad);
      break;
    case IR_JUMP:
      pki_jump(out, quad);
      break;
    case IR_LABEL:
      pki_label(out, quad);
      break;
    case IR_LEAVE:
      pki_leave(out);
      break;
    case IR_MODULO:
      pki_modulo(out, quad);
      break;
    case IR_MULTIPLY:
      pki_reg_op(out, "MUL", quad);
      break;
    case IR_OR:
      pki_reg_op(out, "OR", quad);
      break;
    case IR_PARAM:
      /* TODO */
      break;
    case IR_POP:
      /* TODO */
      break;
    case IR_PUSH:
      pki_push(out, quad);
      break;
    case IR_READ:
      pki_read(out, quad);
      break;
    case IR_REF:
      pki_ref(out, quad);
      break;
    case IR_RETURN:
      pki_return(out, quad);
      break;
    case IR_SUBTRACT:
      pki_reg_op(out, "SUB", quad);
      break;
    case IR_WRITE:
      pki_write(out, quad);
      break;
    case IR_WRITE_LITERAL:
      pki_write_literal(out, quad);
      break;
    case IR_XOR:
      pki_reg_op(out, "XOR", quad);
      break;
    }
  pki_reset_reg();
}

static void pki_globals_callback(SYMBOL *sym, void *data)
{
  FILE *out = (FILE *) data;
  if (symbol_is_function(sym))
    return;
  fprintf(out, "L%i:\tds %i\n", symbol_address(sym), symbol_sizeof(sym));
}

static void pki_literals_callback(LITERAL *lit, void *data)
{
  FILE *out = (FILE *) data;
  fprintf(out, "L%i:\tda \"%s\"\n", literal_address(lit), literal_value(lit));
}

static void pki_generate_prologue(FILE *out)
{
  int i;
  for (i = 0; i < REG_MIN; i++)
    pki_lsc(out, i, i);
}

void pki_generate(FILE *out, IR *ir)
{
  pki_generate_prologue(out);
  ir_each(ir, pki_generate_callback, (void *) out);
}

void pki_generate_globals(FILE *out, SYMBOLS *symbols)
{
  symbol_table_each(symbols, pki_globals_callback, (void *) out);
}

void pki_generate_literals(FILE *out, LITERALS *literals)
{
  literal_table_each(literals, pki_literals_callback, (void *) out);
}
