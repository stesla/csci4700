#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);

void perror_die(const char *msg);

void ext(const char *filename, const char *newext, char *out, size_t size);

#endif
