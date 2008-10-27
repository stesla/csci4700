#ifndef BUFFER_H
#define BUFFER_H

typedef struct _buffer BUFFER;
typedef void (*BUFFER_CALLBACK)(void *entry, void *data);

BUFFER *buffer_create(size_t entry_size, size_t grow_by);
void buffer_each(BUFFER *buffer, BUFFER_CALLBACK callback, void *data);
int buffer_is_empty(BUFFER *buffer);
void *buffer_last_entry(BUFFER *buffer);
void *buffer_next(BUFFER *buffer);

#endif
