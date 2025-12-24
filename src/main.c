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

void prompt_display() {
    char buffer[PATH_MAX];

    if (getcwd(buffer, PATH_MAX) == NULL) {
        fprintf(stderr, "Cannot get current working directory\n");
        exit(EXIT_FAILURE);
    }
    
    printf("%s%s$%s ", BRIGHT_BLUE, buffer, RESET);
}

char* read_input() {
    char *input = (char*)malloc(INPUT_BUFFER_SIZE * sizeof(char));

    if (input == NULL) {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    if (fgets(input, INPUT_BUFFER_SIZE, stdin) == NULL) {
        free(input);
        input = NULL;
        return NULL;
    }

    input[strcspn(input, "\n")] = '\0';

    return input;
}

int main() {
   while (true) {
        prompt_display();
        char *input = read_input();

        if (input == NULL) {
            break;
        }

        free(input); input = NULL;
    }

    return 0;
}
