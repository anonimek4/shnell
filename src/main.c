#include "../include/shnell.h"

int main()
{
    char path[PATH_MAX];

    if (getcwd(path, PATH_MAX) == NULL)
    {
        fprintf(stderr, "%s: getcwd error\n", EXECUTABLE_NAME);
        exit(EXIT_FAILURE);
    }

    if (setenv("SHELL", path, 1) != 0)
    {
        fprintf(stderr, "%s: setenv error\n", EXECUTABLE_NAME);
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        prompt_display();
        char *input = read_input();

        if (input == NULL)
        {
            break;
        }

        Command *cmd = parse(input);
        command_execute(cmd);
        command_free(cmd);
    }

    return 0;
}
