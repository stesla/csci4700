#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

void perror_die(const char *msg);

#endif
