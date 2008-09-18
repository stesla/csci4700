#include <stdio.h>
#include <string.h>
#include "input.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

static FILE *input;
static int listing = 0;
static int lineno = 0;
static char line[MAX_LINE];

int close_input()
{
  return(fclose(input) != 0 ? -1 : 0);
}

size_t my_input(unsigned char *buf, size_t max_size)
{
  size_t size = min(max_size, sizeof(line));

  if (fgets(line, size, input) != NULL)
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

int open_input(const char *filename)
{
  return((input = fopen(filename, "r")) == NULL ? -1 : 0);
}

void toggle_listing()
{
  listing = !listing;
}
