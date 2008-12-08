#ifndef SYMBOL_H
#define SYMBOL_H

typedef struct _symbols SYMBOLS;
typedef struct _symbol SYMBOL;
typedef void (*SYMBOL_CALLBACK)(SYMBOL *, void *);

#define SYMBOL_DIRECTION_DOWN -1
#define SYMBOL_DIRECTION_UP 1

/*
** Creation
*/
SYMBOLS *symbol_table_create(SYMBOLS *table, int direction);
SYMBOL *symbol_table_add_function(SYMBOLS *table, const char *id, void *data);
SYMBOL *symbol_table_add_global(SYMBOLS *table, const char *id);
SYMBOL *symbol_table_add_global_array(SYMBOLS *table, const char *id, size_t size);
SYMBOL *symbol_table_add_local(SYMBOLS *table, const char *id);
SYMBOL *symbol_table_add_param(SYMBOLS *table, const char *id, int is_array);
void symbol_table_begin_scope(SYMBOLS *table);
void symbol_table_end_scope(SYMBOLS *table);

/*
** Lookup
*/
SYMBOL *symbol_table_find(SYMBOLS *table, const char *id);

/*
** Iteration
*/
void symbol_table_each(SYMBOLS *table, SYMBOL_CALLBACK callback, void *data);

/*
** Accessors
*/
/* IF symbol_is_global
 * THEN symbol_address returns a label number
 * ELSE symbol_address returns an offset */
int symbol_address(SYMBOL *symbol);
void *symbol_data(SYMBOL *symbol);
const char *symbol_id(SYMBOL *symbol);
int symbol_is_function(SYMBOL *symbol);
int symbol_is_global(SYMBOL *symbol);
int symbol_size(SYMBOL *symbol);
int symbol_sizeof(SYMBOL *symbol);
size_t symbol_table_size(SYMBOLS *table);

#endif
