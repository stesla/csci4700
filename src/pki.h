#ifndef PKI_H
#define PKI_H

#include <stdio.h>
#include "ir.h"
#include "literal.h"
#include "symbol.h"

void pki_generate(FILE *out, IR *ir);
void pki_generate_globals(FILE *out, SYMBOLS *symbols);
void pki_generate_literals(FILE *out, LITERALS *literals);

#endif
