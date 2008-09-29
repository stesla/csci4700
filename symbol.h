#ifndef SYMBOL_H
#define SYMBOL_H

/* Creation */
void *symbol_table_create(void *table);
void symbol_table_add_global(void *table, const char *id);
void symbol_table_add_global_array(void *table, const char *id, size_t size);
int symbol_table_add_local(void *table, const char *id);
void symbol_table_add_param(void *table, const char *id, int is_array);

/* Lookup */
void *symbol_table_find(void *table, const char *id);

/* Iteration */
void symbol_table_first(void *table, void **cursor);
void symbol_table_next(void **cursor);

/* Accessors */
const char *symbol_id(void *cursor);

#endif SYMBOL_H
