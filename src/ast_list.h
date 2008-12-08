#ifndef AST_LIST_H
#define AST_LIST_H

size_t ast_list_size();
void ast_list_init(NODE *node, va_list args);
size_t ast_list_length(NODE *node);

#endif
