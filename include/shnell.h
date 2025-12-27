#ifndef SHNELL_H
#define SHNELL_H

// LIBRARIES
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// CONSTANTS
#define INPUT_BUFFER_SIZE 1024
#define BRIGHT_BLUE "\033[94m"
#define DELIMS " \t\n"
#define RESET "\033[0m"
#define MALLOC_ERROR "Error: Failed to allocate memory (malloc)"
#define EXECUTABLE_NAME "shnell"

// STRUCT DECLARATIONS
typedef struct
{
    char **argv;
    size_t argv_capacity;
    char *input_file;
    char *output_file;
    bool append;
    bool background;
} Command;

typedef struct
{
    const char *name;
    void (*handler)(Command *cmd);
} InternalCommand;

// FUNCTION PROTOTYPES

// utils.c
void prompt_display();
char *read_input();
Command *command_new();
void command_free(Command *);
Command *parse(char *);
void handle_cd(Command *);
void handle_clr(Command *);
void handle_pause(Command *);
void handle_quit(Command *);
void command_execute(Command *);

#endif