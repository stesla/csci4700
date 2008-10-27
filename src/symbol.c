#include <stdio.h>
#include <string.h>
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

typedef struct _scope SCOPE;
struct _scope {
  ENTRY *head;
  SCOPE *parent;
  TABLE *table;
};

struct _table {
  SCOPE *scope;
  SYMBOL *start;
  SYMBOL *point;
  size_t size;
};

#define E(x) (*((ENTRY *) (x)))

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

static SCOPE *scope_create(TABLE *table, SCOPE *parent)
{
  SCOPE *result = my_malloc(sizeof(SCOPE));
  result->table = table;
  result->parent = parent;
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

#define GROW_BY 64
#define GROW_BYTES (64 * sizeof(SYMBOL))

static void symbol_table_grow(TABLE *table)
{
  size_t new_size = (table->size + GROW_BY) * sizeof(SYMBOL);
  table->start = my_realloc(table->start, new_size);
  table->point = table->start + table->size;
  memset(table->point, 0, GROW_BYTES);
  table->size = table->size + GROW_BY;
}

static int symbol_table_should_grow(TABLE *table)
{
  return (table->start + table->size) == table->point;
}

static SYMBOL *symbol_table_add_symbol(TABLE *table, const char *id)
{
  SYMBOL *symbol;
  size_t address = symbol_table_size(table);

  if (symbol_table_should_grow(table))
    symbol_table_grow(table);

  symbol = table->point++;
  symbol->id = id;
  symbol->count = 1;
  symbol->size = INTEGER_SIZE;
  symbol->address = address;

  table->scope->head = entry_create(symbol, table->scope->head);
  return symbol;
}

void symbol_table_each(TABLE *table, SYMBOL_CALLBACK callback, void *data)
{
  SYMBOL *current = table->start;
  while (current < table->point)
    callback(current++, data);
}

static TABLE *symbol_table_find_global_table(TABLE *table)
{
  SCOPE *scope = table->scope;
  while (scope->parent)
    scope = scope->parent;
  return scope->table;
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

int symbol_size(SYMBOL *symbol)
{
  return symbol->count * symbol->size;
}

TABLE *symbol_table_create(TABLE *table)
{
  TABLE *result = my_malloc(sizeof(TABLE));
  result->scope = scope_create(result, table ? table->scope : NULL);
  return result;
}

SYMBOL *symbol_table_add_global(TABLE *table, const char *id)
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

SYMBOL *symbol_table_add_global_array(TABLE *table, const char *id, size_t count)
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

SYMBOL *symbol_table_add_local(TABLE *table, const char *id)
{
  SYMBOL *result = symbol_table_find(table, id);
  if (result == NULL)
    result = symbol_table_add_symbol((TABLE *) table, id);
  return result;
}

SYMBOL *symbol_table_add_param(TABLE *table, const char *id, int is_array)
{
  SYMBOL *result = symbol_table_add_symbol((TABLE *) table, id);
  /* TODO: Should I be recording a count here? */
  result->is_array = is_array;
  return result;
}

void symbol_table_begin_scope(TABLE *table)
{
  table->scope = scope_create(table, table->scope);
}

void symbol_table_end_scope(TABLE *table)
{
  table->scope = table->scope->parent;
}

SYMBOL *scope_find(SCOPE *scope, const char *id)
{
  ENTRY *head = scope->head;

  /* First check the symbols defined in this scope */
  while (head)
    {
      if (strcmp(head->first->id, id) == 0)
        return head->first;
      else
        head = head->rest;
    }

  /* If we get to here, move on to the parent scope */
  if (scope->parent)
    return scope_find(scope->parent, id);
  else
    return NULL;
}

SYMBOL *symbol_table_find(TABLE *table, const char *id)
{
  scope_find(table->scope, id);
}

size_t symbol_table_size(TABLE *table)
{
  if (table->start == table->point)
    return 0;
  else
    {
      SYMBOL *last = table->point - 1;
      return last->address + last->size * last->count;
    }
}
