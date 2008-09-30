#include <stdio.h>
#include "symbol.h"
#include "util.h"
#include "sizes.h"

typedef struct _symbol {
  const char *id;
  int is_array;
  int count;
  int size;
} SYMBOL;

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

static SYMBOL *symbol_create(const char *id, int is_array, int count, int size)
{
  SYMBOL *result = my_malloc(sizeof(SYMBOL));
  result->id = id;
  result->is_array = is_array;
  result->count = count;
  result->size = size;
  return result;
}

static symbol_table_add_symbol(TABLE *table, const char *id, int is_array, int count, int size)
{
  SYMBOL *symbol = symbol_create(id, FALSE, 1, INTEGER_SIZE);
  T(table).head = entry_create(symbol, T(table).head);
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

const char *symbol_id(void *cursor)
{
  return E(cursor).first->id;
}

void *symbol_table_create(void *table)
{
  TABLE *result = my_malloc(sizeof(TABLE));
  result->parent = table;
  return result;
}


void symbol_table_add_global(void *table, const char *id)
{
  TABLE *globals = symbol_table_find_global_table(table);

  if (symbol_table_find(globals, id))
    return;

  symbol_table_add_symbol(globals, id, FALSE, 1, INTEGER_SIZE);
}

void symbol_table_add_global_array(void *table, const char *id, size_t count)
{
  TABLE *globals = symbol_table_find_global_table(table);

  if (symbol_table_find(globals, id))
    return;

  symbol_table_add_symbol(globals, id, TRUE, count, INTEGER_SIZE);
}

int symbol_table_add_local(void *table, const char *id)
{
  if (symbol_table_find(table, id))
    return FALSE;

  symbol_table_add_symbol((TABLE *) table, id, FALSE, 1, INTEGER_SIZE);
  return TRUE;
}

void symbol_table_add_param(void *table, const char *id, int is_array)
{
  /* TODO: Should I be recording a count here? */
  symbol_table_add_symbol((TABLE *) table, id, is_array, 1, INTEGER_SIZE);
}

void *symbol_table_find(void *table, const char *id)
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

void symbol_table_first(void *table, void **cursor)
{
  *cursor = T(table).head;
}

void symbol_table_next(void **cursor)
{
  *cursor = E(*cursor).rest;
}