#include "pki.h"

static void pki_lsc(FILE *out, int reg, int val)
{
  fprintf(out, "\tLSC R%i,%i\n", reg, val);
}

static void pki_syscall(FILE *out, int r1, int r2)
{
  fprintf(out, "\tSYSCALL R%i,R%i\n", r1, r2);
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
  pki_generate_epilogue(out);
}
