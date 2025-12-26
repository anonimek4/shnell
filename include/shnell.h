#ifndef SHNELL_H
#define SHNELL_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INPUT_BUFFER_SIZE 1024
#define BRIGHT_BLUE "\033[94m"
#define DELIMS " \t\n"
#define RESET "\033[0m"
#define MALLOC_ERROR "Error: Failed to allocate memory (malloc)"
#define EXECUTABLE_NAME "shnell"

typedef struct
{
    char **argv;
    size_t argv_capacity;
    char *input_file;
    char *output_file;
    bool append;
    bool background;
} Command;

void prompt_display();
char *read_input();
Command *parse(char *);

//utils.c
Command* command_new();
void command_free(Command*);


#endif