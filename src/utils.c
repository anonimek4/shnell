#include "../include/shnell.h"

InternalCommand internal_commands[] = {
    {"cd", handle_cd},
    {"clr", handle_clr},
    {"pause", handle_pause},
    {"quit", handle_quit},
    {NULL, NULL},
};

void prompt_display()
{
    char buffer[PATH_MAX];

    if (getcwd(buffer, PATH_MAX) == NULL)
    {
        fprintf(stderr, "%s: %s\n", EXECUTABLE_NAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("%s%s$%s ", BRIGHT_BLUE, buffer, RESET);
}

char *read_input()
{
    char *input = (char *)malloc(INPUT_BUFFER_SIZE * sizeof(char));

    if (input == NULL)
    {
        fprintf(stderr, "%s: %s\n", EXECUTABLE_NAME, strerror(errno));
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

Command *command_new()
{
    Command *cmd = (Command *)malloc(sizeof(Command));

    if (cmd == NULL)
    {
        fprintf(stderr, "%s: %s\n", EXECUTABLE_NAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    cmd->argv_capacity = 8;
    cmd->argv = (char **)malloc(cmd->argv_capacity * sizeof(char *));

    if (cmd->argv == NULL)
    {
        fprintf(stderr, "%s: %s\n", EXECUTABLE_NAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

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
                fprintf(stderr, "%s: %s\n", EXECUTABLE_NAME, strerror(errno));
                exit(EXIT_FAILURE);
            }

            cmd->argv = temp;
        }

        if (strcmp(token, "<") == 0)
        {
            token = strtok(NULL, DELIMS);

            if (token == NULL)
            {
                fprintf(stderr, "%s: Missing file name near '<'\n", EXECUTABLE_NAME);
                command_free(cmd);
                return NULL;
            }

            cmd->input_file = strdup(token);

            if (cmd->input_file == NULL)
            {
                fprintf(stderr, "%s: %s\n", EXECUTABLE_NAME, strerror(errno));
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

void handle_cd(Command *cmd)
{
    if (cmd->argv[1] == NULL)
    {
        fprintf(stderr, "Usage: cd <dir_name>\n");
    }
    else if (chdir(cmd->argv[1]) != 0)
    {
        fprintf(stderr, "%s: cd: %s: No such file or directory\n", EXECUTABLE_NAME, cmd->argv[1]);
    }
}

void handle_clr(Command *)
{
    printf("\033[2J\033[H");
}

void handle_pause(Command *)
{
    char buffer[256];
    printf("Press Enter to continue...");
    fgets(buffer, sizeof(buffer), stdin);
}

void handle_quit(Command *)
{
    exit(EXIT_SUCCESS);
}

void command_execute(Command *cmd)
{
    if (cmd->argv[0] == NULL)
        return;

    for (InternalCommand *ic = internal_commands; ic->name != NULL; ic++)
    {
        if (strcmp(cmd->argv[0], ic->name) == 0)
        {
            ic->handler(cmd);
            return;
        }
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        fprintf(stderr, "%s: %s: %s\n", EXECUTABLE_NAME, cmd->argv[0], strerror(errno));
        return;
    }
    else if (pid == 0)
    {
        // child proc

        if (cmd->input_file != NULL)
        {
            int fd = open(cmd->input_file, O_RDONLY);

            if (fd == -1)
            {
                fprintf(stderr, "%s: %s: %s\n", EXECUTABLE_NAME, cmd->input_file, strerror(errno));
                return;
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (cmd->output_file != NULL)
        {
            int flags = O_WRONLY | O_CREAT;
            flags = cmd->append ? flags | O_APPEND : flags | O_TRUNC;
            mode_t mode = 0644;

            int fd = open(cmd->output_file, flags, mode);

            if (fd == -1)
            {
                fprintf(stderr, "%s: %s: %s\n", EXECUTABLE_NAME, cmd->output_file, strerror(errno));
                return;
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd->argv[0], cmd->argv);

        fprintf(stderr, "%s: %s: %s\n", EXECUTABLE_NAME, cmd->argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        if (!cmd->background)
        {
            int status;
            waitpid(pid, &status, 0);
        }
        else
        {
            printf("[%d]\n", pid);
        }
    }
}