#ifndef SYMBOL_H
#define SYMBOL_H

typedef struct _table TABLE;
typedef struct _symbol SYMBOL;

/*
** Creation
*/
TABLE *symbol_table_create(TABLE *table);
SYMBOL *symbol_table_add_global(TABLE *table, const char *id);
SYMBOL *symbol_table_add_global_array(TABLE *table, const char *id, size_t size);
SYMBOL *symbol_table_add_local(TABLE *table, const char *id);
SYMBOL *symbol_table_add_param(TABLE *table, const char *id, int is_array);
void symbol_table_begin_scope(TABLE *table);
void symbol_table_end_scope(TABLE *table);

/*
** Lookup
*/
SYMBOL *symbol_table_find(TABLE *table, const char *id);

/*
** Accessors
*/
/* IF symbol_is_global
 * THEN symbol_address returns a label number
 * ELSE symbol_address returns an offset */
int symbol_address(SYMBOL *symbol);
const char *symbol_id(SYMBOL *symbol);
int symbol_is_global(SYMBOL *symbol);
size_t symbol_table_size(TABLE *table);

#endif
