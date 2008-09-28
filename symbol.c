#include <stdio.h>
#include "symbol.h"

void *symbol_table_create(void *table)
{
  printf("symbol_table_create(%p)\n", table);
}

void symbol_table_add_array(void *table, const char *id, size_t size)
{
  printf("symbol_table_add_array(%p, \"%s\", %i)\n", table, id, size);
}

void symbol_table_add_local(void *table, const char *id)
{
  printf("symbol_table_add_local(%p, \"%s\")\n", table, id);
}

void symbol_table_add_global(void *table, const char *id)
{
  printf("symbol_table_add_global(%p, \"%s\")", table, id);
}
