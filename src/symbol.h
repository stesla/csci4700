#ifndef SYMBOL_H
#define SYMBOL_H

typedef struct _symbol SYMBOL;

/*
** Creation
*/
void *symbol_table_create(void *table);
SYMBOL *symbol_table_add_global(void *table, const char *id);
SYMBOL *symbol_table_add_global_array(void *table, const char *id, size_t size);
SYMBOL *symbol_table_add_local(void *table, const char *id);
SYMBOL *symbol_table_add_param(void *table, const char *id, int is_array);

/*
** Lookup
*/
SYMBOL *symbol_table_find(void *table, const char *id);

/*
** Accessors
*/
/* IF symbol_is_global
 * THEN symbol_address returns a label number
 * ELSE symbol_address returns an offset */
int symbol_address(SYMBOL *symbol);
const char *symbol_id(SYMBOL *symbol);
int symbol_is_global(SYMBOL *symbol);
size_t symbol_table_size(void *table);

#endif
