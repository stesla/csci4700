#ifndef LITERAL_H
#define LITERAL_H

typedef struct _literals LITERALS;
typedef struct _literal LITERAL;
typedef void (*LITERAL_CALLBACK)(LITERAL *, void *);

int literal_address(LITERAL *literal);
const char *literal_value(LITERAL *literal);

LITERALS *literal_table_create();
LITERAL *literal_table_add(LITERALS *literals, const char *text);
void literal_table_each(LITERALS *literals, LITERAL_CALLBACK callback,
                        void *data);

#endif
