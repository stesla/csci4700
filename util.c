#include <stdlib.h>
#include "util.h"

void *my_malloc(size_t size)
{
  void *result;
  if ((result = malloc(size)) == NULL)
    {
      perror("malloc");
      exit(1);
    }
  return result;
}
