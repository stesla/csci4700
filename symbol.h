#ifndef SYMBOL_H
#define SYMBOL_H

void *symbol_table_create(void *table);
void symbol_table_add_array(void *table, const char *id, size_t size);
void symbol_table_add_local(void *table, const char *id);
void symbol_table_add_global(void *table, const char *id);

#endif SYMBOL_H
