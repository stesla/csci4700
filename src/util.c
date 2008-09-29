#include <stdlib.h>
#include <strings.h>
#include "util.h"

void *my_malloc(size_t size)
{
  void *result;
  if ((result = malloc(size)) == NULL)
    {
      perror("malloc");
      exit(1);
    }
  bzero(result, size);
  return result;
}
