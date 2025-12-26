#include "../include/shnell.h"

void prompt_display()
{
    char buffer[PATH_MAX];

    if (getcwd(buffer, PATH_MAX) == NULL)
    {
        fprintf(stderr, "Cannot get current working directory\n");
        exit(EXIT_FAILURE);
    }

    printf("%s%s$%s ", BRIGHT_BLUE, buffer, RESET);
}

char *read_input()
{
    char *input = (char *)malloc(INPUT_BUFFER_SIZE * sizeof(char));

    if (input == NULL)
    {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    if (fgets(input, INPUT_BUFFER_SIZE, stdin) == NULL)
    {
        free(input);
        input = NULL;
        return NULL;
    }

    input[strcspn(input, "\n")] = '\0';

    return input;
}

int main()
{
    while (true)
    {
        prompt_display();
        char *input = read_input();

        if (input == NULL)
        {
            break;
        }

        Command *cmd = parse(input);
        command_free(cmd);
    }

    return 0;
}
