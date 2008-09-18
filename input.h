#ifndef INPUT_H
#define INPUT_H

#define MAX_LINE 160

int close_input();
const char *input_file();
int input_lineno();
size_t my_input(unsigned char *buf, size_t max_size);
int open_input(const char *filename);
void toggle_listing();

#endif INPUT_H
