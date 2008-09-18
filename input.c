#include <stdio.h>
#include <string.h>
#include "input.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

extern FILE *yyin;

static int listing = 0;
static int lineno = 0;
static char line[MAX_LINE];

size_t my_input(unsigned char *buf, size_t max_size)
{
  size_t size = min(max_size, sizeof(line));

  if (fgets(line, size, yyin) != NULL)
    {
      size_t length = strlen(line);

      lineno++;

      strncpy((char *) buf, line, size);

      if (listing)
        printf("%d: %s", lineno, line);

      return(length);
    }
  else
    {
      *buf = line[0] = '\0';
      return(0);
    }
}

void toggle_listing()
{
  listing = !listing;
}
