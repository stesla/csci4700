#include <stdio.h>
#include "symbol.h"
#include "util.h"
#include "sizes.h"

struct _symbol {
  const char *id;
  int is_array;
  int is_global;
  int count;
  int size;
  int address; /* IF is_global THEN label number ELSE offset */
};

typedef struct _entry ENTRY;
struct _entry {
  SYMBOL *first;
  ENTRY *rest;
};

typedef struct _table TABLE;
struct _table {
  ENTRY *head;
  TABLE *parent;
};

#define E(x) (*((ENTRY *) (x)))
#define T(x) (*((TABLE *) (x)))

/*
** Private Functions
*/

static ENTRY *entry_create(SYMBOL *first, ENTRY *rest)
{
  ENTRY *result = my_malloc(sizeof(ENTRY));
  result->first = first;
  result->rest = rest;
  return result;
}

static SYMBOL *symbol_create(const char *id)
{
  SYMBOL *result = my_malloc(sizeof(SYMBOL));
  result->id = id;
  result->count = 1;
  result->size = INTEGER_SIZE;
  return result;
}

static SYMBOL *symbol_table_add_symbol(TABLE *table, const char *id)
{
  SYMBOL *symbol = symbol_create(id);
  T(table).head = entry_create(symbol, T(table).head);
  return symbol;
}

static TABLE *symbol_table_find_global_table(void *table)
{
  TABLE *result = (TABLE *) table;
  while (result->parent)
    result = result->parent;
  return result;
}

/*
** Public Functions
*/

int symbol_address(SYMBOL *symbol)
{
  return symbol->address;
}

const char *symbol_id(SYMBOL *symbol)
{
  return symbol->id;
}

int symbol_is_global(SYMBOL *symbol)
{
  return symbol->is_global;
}

void *symbol_table_create(void *table)
{
  TABLE *result = my_malloc(sizeof(TABLE));
  result->parent = table;
  return result;
}

SYMBOL *symbol_table_add_global(void *table, const char *id)
{
  TABLE *globals = symbol_table_find_global_table(table);
  SYMBOL *result = symbol_table_find(table, id);
  if (result == NULL)
    {
      result = symbol_table_add_symbol(globals, id);
      result->is_global = TRUE;
      result->address = ir_make_label();
    }
  return result;
}

SYMBOL *symbol_table_add_global_array(void *table, const char *id, size_t count)
{
  TABLE *globals = symbol_table_find_global_table(table);
  SYMBOL *result = symbol_table_find(table, id);
  if (result == NULL)
    {
      result = symbol_table_add_symbol(globals, id);
      result->is_array = result->is_global = TRUE;
      result->count = count;
      result->address = ir_make_label();
    }
  return result;
}

SYMBOL *symbol_table_add_local(void *table, const char *id)
{
  SYMBOL *result = symbol_table_find(table, id);
  if (result == NULL)
    {
      size_t size = symbol_table_size(table);
      result = symbol_table_add_symbol((TABLE *) table, id);
      result->address = size;
    }
  return result;
}

SYMBOL *symbol_table_add_param(void *table, const char *id, int is_array)
{
  SYMBOL *result = symbol_table_add_symbol((TABLE *) table, id);
  /* TODO: Should I be recording a count here? */
  result->is_array = is_array;
  return result;
}

SYMBOL *symbol_table_find(void *table, const char *id)
{
  ENTRY *head = T(table).head;

  /* First check the symbols defined in this scope */
  while (head)
    {
      if (strcmp(head->first->id, id) == 0)
        return head->first;
      else
        head = head->rest;
    }

  /* If we get to here, move on to the parent scope */
  if (T(table).parent)
    return symbol_table_find(T(table).parent, id);
  else
    return NULL;
}

size_t symbol_table_size(void *table)
{
  /* The size of the table should be the address of the most recent symbol in
   * the table plus its total size. */
  TABLE *tab = (TABLE *) table;
  SYMBOL *sym = tab->head ? tab->head->first : NULL;
  return sym ? sym->address + sym->count * sym->size : 0;
}
