#include <stdlib.h>
#include <strings.h>
#include "util.h"

void perror_die(const char *msg)
{
  perror(msg);
  exit(1);
}

void *my_malloc(size_t size)
{
  void *result;
  if ((result = malloc(size)) == NULL)
    perror_die("malloc");
  bzero(result, size);
  return result;
}

void *my_realloc(void *ptr, size_t size)
{
  void *result;
  if ((result = realloc(ptr, size)) == NULL)
    perror_die("malloc");
  return result;
}
