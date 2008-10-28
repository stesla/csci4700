#include <string.h>
#include "buffer.h"
#include "ir.h"
#include "literal.h"
#include "util.h"

struct _literals {
  BUFFER *buffer;
};

struct _literal {
  char *value;
  int address;
};

int literal_address(LITERAL *literal)
{
  return literal->address;
}

const char *literal_value(LITERAL *literal)
{
  return literal->value;
}

LITERALS *literal_table_create()
{
  LITERALS *result = my_malloc(sizeof(LITERALS));
  result->buffer = buffer_create(sizeof(LITERAL), 16);
  return result;
}

LITERAL *literal_table_add(LITERALS *literals, const char *text)
{
  LITERAL *literal = buffer_next(literals->buffer);
  literal->value = strdup(text);
  literal->address = ir_make_label();
  return literal;
}

void literal_table_each(LITERALS *literals, LITERAL_CALLBACK callback,
                        void *data)
{
  buffer_each(literals->buffer, (BUFFER_CALLBACK) callback, data);
}
