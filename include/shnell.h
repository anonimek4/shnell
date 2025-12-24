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
#define RESET "\033[0m"

void prompt_display();
char* read_input();

#endif 