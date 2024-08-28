#include "simple_shell.h"

/**
 * shell_loop - Main loop for the shell.
 * @program_name: The name of the program (argv[0]).
 */
void shell_loop(char *program_name)
{
    char *line;
    char *args[MAX_ARGS];
    int status = 1;

    while (status)
    {
        printf("$ ");
        line = read_line();
        if (line == NULL)
        {
            if (feof(stdin))
            {
                printf("\n");
                break;
            }
            perror(program_name);
            continue;
        }

        tokenize(line, args);
        status = execute_command(args, program_name);

        free(line);
    }
}

/**
 * read_line - Reads a line from stdin.
 * 
 * Return: The input string.
 */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            free(line);
            return (NULL);
        }
        else
        {
            perror("read_line");
            free(line);
            exit(EXIT_FAILURE);
        }
    }

    return (line);
}

/**
 * tokenize - Splits a string into an array of arguments.
 * @line: The input line to split.
 * @args: The array to store the arguments.
 */
void tokenize(char *line, char **args)
{
    char *token;
    int i = 0;

    token = strtok(line, DELIM);
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, DELIM);
    }
    args[i] = NULL;
}

/**
 * execute_command - Executes a command.
 * @args: Array of arguments.
 * @program_name: The name of the program (argv[0]).
 * 
 * Return: 1 if the shell should continue running, 0 if it should terminate.
 */
int execute_command(char **args, char *program_name)
{
    pid_t pid;
    int status;

    if (args[0] == NULL)
    {
        return (1); /* Empty command */
    }

    pid = fork();
    if (pid == 0)
    {
        if (execve(args[0], args, environ) == -1)
        {
            perror(program_name);
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        wait(&status);
    }

    return (1);
}
