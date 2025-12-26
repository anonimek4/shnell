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

Command *parse(char *input)
{
    Command *cmd = (Command *)malloc(sizeof(Command));

    if (cmd == NULL)
    {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    cmd->argv_capacity = 8;
    cmd->argv = (char **)malloc(cmd->argv_capacity * sizeof(char *));

    if (cmd->argv == NULL)
    {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }

    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = false;
    cmd->background = false;

    char *token = strtok(input, DELIMS);
    size_t i = 0;

    while (token != NULL)
    {
        if (i >= cmd->argv_capacity)
        {
            cmd->argv_capacity *= 2;

            char **temp = (char **)realloc(cmd->argv, cmd->argv_capacity * sizeof(char *));

            if (temp == NULL)
            {
                fprintf(stderr, "Cannot reallocate memory\n");
                exit(EXIT_FAILURE);
            }

            cmd->argv = temp;
        }

        if (strcmp(token, "<") == 0)
        {
            token = strtok(NULL, DELIMS);
            cmd->input_file = token;
        }
        else if (strcmp(token, ">") == 0)
        {
            token = strtok(NULL, DELIMS);
            cmd->output_file = token;
            cmd->append = false;
        }
        else if (strcmp(token, ">>") == 0)
        {
            token = strtok(NULL, DELIMS);
            cmd->output_file = token;
            cmd->append = true;
        }
        else if (strcmp(token, "&") == 0)
        {
            cmd->background = true;
        }
        else
        {
            cmd->argv[i] = strdup(token);
            i++;
        }

        token = strtok(NULL, DELIMS);
    }

    cmd->argv[i] = NULL;

    return cmd;
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

        for (size_t i = 0; i < cmd->argv_capacity; i++)
        {
            if (cmd->argv[i] == NULL)
            {
                break;
            }

            printf("%s\n", cmd->argv[i]);
        }
    }

    return 0;
}
