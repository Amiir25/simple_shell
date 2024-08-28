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
        line = custom_getline();
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

        custom_tokenize(line, args);
        status = execute_command(args, program_name);

        free(line);
    }
}

/**
 * custom_getline - Custom implementation of getline function.
 * 
 * Return: The input string.
 */
char *custom_getline(void)
{
    size_t bufsize = MAX_LINE_LEN;
    size_t position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer)
    {
        perror("Unable to allocate buffer");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_LINE_LEN;
            buffer = realloc(buffer, bufsize);
            if (!buffer)
            {
                perror("Unable to allocate buffer");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/**
 * custom_tokenize - Splits a string into an array of arguments.
 * @line: The input line to split.
 * @args: The array to store the arguments.
 */
void custom_tokenize(char *line, char **args)
{
    int i = 0;
    char *start = line;
    char *end;

    while (*start)
    {
        while (*start && strchr(DELIM, *start))
            start++;

        end = start;
        while (*end && !strchr(DELIM, *end))
            end++;

        if (start == end)
            break;

        args[i++] = strndup(start, end - start);
        start = end;
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

    /* Handle the exit built-in */
    if (strcmp(args[0], "exit") == 0)
    {
        if (args[1])
            exit(atoi(args[1]));
        return (0);
    }

    /* Handle the env built-in */
    if (strcmp(args[0], "env") == 0)
    {
        print_env();
        return (1);
    }

    /* Handle the setenv built-in */
    if (strcmp(args[0], "setenv") == 0)
    {
        if (args[1] && args[2])
            set_env(args[1], args[2]);
        else
            fprintf(stderr, "%s: setenv: Missing arguments\n", program_name);
        return (1);
    }

    /* Handle the unsetenv built-in */
    if (strcmp(args[0], "unsetenv") == 0)
    {
        if (args[1])
            unset_env(args[1]);
        else
            fprintf(stderr, "%s: unsetenv: Missing argument\n", program_name);
        return (1);
    }

    /* Handle the cd built-in */
    if (strcmp(args[0], "cd") == 0)
    {
        change_directory(args[1]);
        return (1);
    }

    /* Check if command is in PATH */
    char *cmd_path = find_command(args[0]);
    if (cmd_path == NULL)
    {
        fprintf(stderr, "%s: command not found: %s\n", program_name, args[0]);
        return (1);
    }

    pid = fork();
    if (pid == 0)
    {
        if (execve(cmd_path, args, environ) == -1)
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

    free(cmd_path);
    return (1);
}

/**
 * set_env - Initialize or modify an environment variable.
 * @name: Name of the environment variable.
 * @value: Value to set.
 */
void set_env(const char *name, const char *value)
{
    if (setenv(name, value, 1) == -1)
    {
        perror("setenv");
    }
}

/**
 * unset_env - Remove an environment variable.
 * @name: Name of the environment variable.
 */
void unset_env(const char *name)
{
    if (unsetenv(name) == -1)
    {
        perror("unsetenv");
    }
}

/**
 * change_directory - Changes the current directory.
 * @path: Path to the directory.
 */
void change_directory(const char *path)
{
    char *new_path;
    char cwd[MAX_PATH_LEN];

    if (!path || strcmp(path, "~") == 0)
    {
        new_path = getenv("HOME");
    }
    else if (strcmp(path, "-") == 0)
    {
        new_path = getenv("OLDPWD");
    }
    else
    {
        new_path = strdup(path);
    }

    if (chdir(new_path) != 0)
    {
        perror("cd");
    }
    else
    {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", getcwd(cwd, sizeof(cwd)), 1);
    }

    if (new_path != path)
        free(new_path);
}
