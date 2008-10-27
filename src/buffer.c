#include <strings.h>
#include "buffer.h"
#include "util.h"

struct _buffer {
  void *start;
  void *point;
  size_t count;
  size_t entry_size;
  size_t grow_by;
};

static int buffer_should_grow(BUFFER *buffer)
{
  return buffer->start + buffer->count * buffer->entry_size == buffer->point;
}

static void buffer_grow(BUFFER *buffer)
{
  size_t grow_size = (buffer->count + buffer->grow_by) * buffer->entry_size;
  buffer->start = my_realloc(buffer->start, grow_size);
  buffer->point = buffer->start + buffer->count * buffer->entry_size;
  bzero(buffer->point, buffer->grow_by * buffer->entry_size);
  buffer->count += buffer->grow_by;
}

BUFFER *buffer_create(size_t entry_size, size_t grow_by)
{
  BUFFER *result = my_malloc(sizeof(BUFFER));
  result->entry_size = entry_size;
  result->grow_by = grow_by;
  return result;
}

void buffer_each(BUFFER *buffer, BUFFER_CALLBACK callback, void *data)
{
  void *current = buffer->start;
  while (current < buffer->point)
    {
      callback(current, data);
      current += buffer->entry_size;
    }
}

int buffer_is_empty(BUFFER *buffer)
{
  return buffer->start == buffer->point;
}

void *buffer_last_entry(BUFFER *buffer)
{
  return buffer->point - buffer->entry_size;
}

void *buffer_next(BUFFER *buffer)
{
  void *result;;
  if (buffer_should_grow(buffer))
    buffer_grow(buffer);
  result = buffer->point;
  buffer->point += buffer->entry_size;
  return result;
}
