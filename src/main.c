#include "../include/shnell.h"

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

char** parse(char *input) {
    int buffer_size = 64;
    int position = 0;
    char **tokens = (char**)malloc(buffer_size * sizeof(char*));
    char *token;

    if (tokens == NULL) {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, DELIMS);

    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= buffer_size) {
            buffer_size *= 2;

            char **temp = (char**)realloc(tokens,  buffer_size * sizeof(char*));

            if(temp == NULL) {
                fprintf(stderr, "Cannot reallocate memory\n");
                exit(EXIT_FAILURE);
            }

            tokens = temp;
        }

        token = strtok(NULL, DELIMS);
    }

    tokens[position] = NULL;

    return tokens;
}

int main() {
   while (true) {
        prompt_display();
        char *input = read_input();

        if (input == NULL) {
            break;
        }

        char **args = parse(input);

        free(input); input = NULL;
        free(args); args = NULL;
    }

    return 0;
}
