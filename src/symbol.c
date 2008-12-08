#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "symbol.h"
#include "util.h"
#include "sizes.h"

struct _symbol {
  const char *id;
  int is_array;
  int is_global;
  int is_function;
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
  SYMBOLS *table;
};

struct _symbols {
  int direction;
  SCOPE *scope;
  BUFFER *buffer;
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

static SCOPE *scope_create(SYMBOLS *table, SCOPE *parent)
{
  SCOPE *result = my_malloc(sizeof(SCOPE));
  result->table = table;
  result->parent = parent;
  return result;
}

static SYMBOL *symbol_table_add_symbol(SYMBOLS *table, const char *id)
{
  SYMBOL *symbol;
  size_t address = symbol_table_size(table) + INTEGER_SIZE;

  symbol = buffer_next(table->buffer);

  symbol->id = id;
  symbol->count = 1;
  symbol->size = INTEGER_SIZE;
  symbol->address = table->direction * address;

  table->scope->head = entry_create(symbol, table->scope->head);
  return symbol;
}

void symbol_table_each(SYMBOLS *table, SYMBOL_CALLBACK callback, void *data)
{
  buffer_each(table->buffer, (BUFFER_CALLBACK) callback, data);
}

static SYMBOLS *symbol_table_find_global_table(SYMBOLS *table)
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

int symbol_is_function(SYMBOL *symbol)
{
  return symbol->is_function;
}

int symbol_is_global(SYMBOL *symbol)
{
  return symbol->is_global;
}

int symbol_size(SYMBOL *symbol)
{
  return symbol->size;
}

int symbol_sizeof(SYMBOL *symbol)
{
  return symbol->count * symbol->size;
}

SYMBOLS *symbol_table_create(SYMBOLS *table, int direction)
{
  SYMBOLS *result = my_malloc(sizeof(SYMBOLS));
  result->buffer = buffer_create(sizeof(SYMBOL), 64);
  result->scope = scope_create(result, table ? table->scope : NULL);
  result->direction = direction;
  return result;
}

SYMBOL *symbol_table_add_function(SYMBOLS *table, const char *id)
{
  /* This function probably will never get called with a table OTHER than the
   * global table. However, it can't hurt to make sure we have it. */
  SYMBOLS *globals = symbol_table_find_global_table(table);
  SYMBOL *result = symbol_table_find(table, id);
  if (result == NULL)
    {
      result = symbol_table_add_symbol(globals, id);
      result->is_function = result->is_global = TRUE;
      result->address = ir_make_label();
      /* TODO: we also need to put the params on this guy */
    }
  return result;
}

SYMBOL *symbol_table_add_global(SYMBOLS *table, const char *id)
{
  SYMBOLS *globals = symbol_table_find_global_table(table);
  SYMBOL *result = symbol_table_find(table, id);
  if (result == NULL)
    {
      result = symbol_table_add_symbol(globals, id);
      result->is_global = TRUE;
      result->address = ir_make_label();
    }
  return result;
}

SYMBOL *symbol_table_add_global_array(SYMBOLS *table, const char *id, size_t count)
{
  SYMBOLS *globals = symbol_table_find_global_table(table);
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

SYMBOL *symbol_table_add_local(SYMBOLS *table, const char *id)
{
  SYMBOL *result = symbol_table_find(table, id);
  if (result == NULL)
    result = symbol_table_add_symbol((SYMBOLS *) table, id);
  return result;
}

SYMBOL *symbol_table_add_param(SYMBOLS *table, const char *id, int is_array)
{
  SYMBOL *result = symbol_table_add_symbol((SYMBOLS *) table, id);
  /* TODO: Should I be recording a count here? */
  result->is_array = is_array;
  return result;
}

void symbol_table_begin_scope(SYMBOLS *table)
{
  table->scope = scope_create(table, table->scope);
}

void symbol_table_end_scope(SYMBOLS *table)
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

SYMBOL *symbol_table_find(SYMBOLS *table, const char *id)
{
  scope_find(table->scope, id);
}

size_t symbol_table_size(SYMBOLS *table)
{
  if (buffer_is_empty(table->buffer))
    return 0;
  else
    {
      SYMBOL *last = buffer_last_entry(table->buffer);
      return abs(last->address) + last->size * last->count;
    }
}
