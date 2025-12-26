#include "../include/shnell.h"

Command *command_new()
{
    Command *cmd = (Command *)malloc(sizeof(Command));

    if (cmd == NULL)
    {
        perror(MALLOC_ERROR);
        exit(EXIT_FAILURE);
    }

    cmd->argv_capacity = 8;
    cmd->argv = (char **)malloc(cmd->argv_capacity * sizeof(char *));
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = false;
    cmd->background = false;

    return cmd;
}

void command_free(Command *cmd)
{
    if (cmd == NULL)
    {
        return;
    }

    if (cmd->argv != NULL)
    {
        for (size_t i = 0; cmd->argv[i] != NULL; i++)
            free(cmd->argv[i]);
        free(cmd->argv);
    }

    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd);
}

Command *parse(char *input)
{
    Command *cmd = command_new();

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

            if (token == NULL)
            {
                fprintf(stderr, "%s: missing file name near '<'\n", EXECUTABLE_NAME);
                command_free(cmd);
                return NULL;
            }

            cmd->input_file = strdup(token);

            if (cmd->input_file == NULL)
            {
                perror(MALLOC_ERROR);
                exit(EXIT_FAILURE);
            }
        }
        else if (strcmp(token, ">") == 0)
        {
            token = strtok(NULL, DELIMS);
            cmd->output_file = strdup(token);
            cmd->append = false;
        }
        else if (strcmp(token, ">>") == 0)
        {
            token = strtok(NULL, DELIMS);
            cmd->output_file = strdup(token);
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